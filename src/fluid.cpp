#include "fluid.hpp"
#include <ranges>

void Fluid::step(float deltaTime)
{
  if (deltaTime <= 0.f)
    return;
  if (params.isGravity)
  {
    for (auto &p : particles)
    {
      p.velocity += Vec(0.f, -deltaTime * GRAVITY);
    }
  }
  applyViscosity(deltaTime);
  for (auto &p : particles)
  {
    p.positionPrev = p.position;
    p.position += deltaTime * p.velocity;
  }
  doubleDensityRelaxation(deltaTime);
  for (auto &p : particles)
  {
    p.velocity = (p.position - p.positionPrev) / deltaTime;
    boundryCollision(p);
  }
}

float kernel(const float dist, const float radius)
{
  if (dist > radius)
    return 0.f;
  return std::pow(1 - dist / radius, 2);
}

float nearKernel(const float dist, const float radius)
{
  if (dist > radius)
    return 0.f;
  return std::pow(1 - dist / radius, 3);
}

float Fluid::smoothingNearKernel(const float dist) const
{
  return nearKernel(dist, params.smoothingRadius);
}

float Fluid::computeNearDensity(const Particle &p) const
{
  float density = 0.f;
  for (const auto &j : particles)
  {
    if (&j != &p)
    {
      float dist = (p.position - j.position).mag();
      density += smoothingNearKernel(dist);
    }
  }
  return density;
}

float Fluid::smoothingKernel(const float dist) const
{
  return kernel(dist, params.smoothingRadius);
}

float Fluid::computeDensity(const Particle &p) const
{
  float density = 0.f;
  for (const auto &j : particles)
  {
    if (&j != &p)
    {
      float dist = (p.position - j.position).mag();
      density += smoothingKernel(dist);
    }
  }
  return density;
}

inline float Fluid::computeNearPseudoPressure(const float nearDensity) const
{
  return params.nearPressureMultiplier * nearDensity;
}

inline float Fluid::computePseudoPressure(const float density) const
{
  return params.pressureMultiplier * (density - params.targetDensity);
}

Vec Fluid::relaxationDisplacement(const Particle &i,
                                  const Particle &j,
                                  const float deltaTime,
                                  const float pressure,
                                  const float nearPressure) const
{
  Vec rij = i.position - j.position;
  if (rij.mag() == 0.f)
  {
    return Vec(0, 0);
  }
  Vec rijUnit = rij / rij.mag();
  return rijUnit * (pressure * (1.f - rij.mag() / params.smoothingRadius) +
                    nearPressure * std::pow(1.f - rij.mag() / params.smoothingRadius, 2));
}
void Fluid::doubleDensityRelaxation(float deltaTime)
{
  for (auto &i : particles)
  {
    Vec predicted = i.position + deltaTime * i.velocity;
    float density = computeDensity(i);
    float pressure = computePseudoPressure(density);
    float nearDensity = computeNearDensity(i);
    float nearPressure = computeNearPseudoPressure(nearDensity);
    Vec dx(0, 0);
    for (auto &j : particles)
    {
      float q = (i.position - j.position).mag() / params.smoothingRadius;
      if (&j != &i && q < 1.f)
      {
        Vec displacement = relaxationDisplacement(i, j, deltaTime, pressure, nearPressure);
        j.position -= displacement / 2.f;
        dx += displacement / 2.f;
      }
    }
    i.position += dx;
  }
}

void Fluid::applyForce(Vec &p, float force, float radius)
{
  for (auto &particle : particles)
  {
    Vec offset = particle.position - p;
    float dist = offset.mag();
    if (dist < radius)
    {
      float influence = kernel(dist, radius);
      Vec forceVec = (offset / dist) * influence * force;
      particle.velocity += forceVec;
    }
  }
}

void Fluid::boundryCollision(Particle &a)
{
  Vec minBound = Vec(0, 0) + Vec(radius, radius);
  Vec maxBound = boundSize - Vec(radius, radius);
  // X-axis collision
  if (a.position.x < minBound.x)
  {
    a.position.x = minBound.x;
    a.velocity.x = -a.velocity.x * params.collisionDamping;
  }
  else if (a.position.x > maxBound.x)
  {
    a.position.x = maxBound.x;
    a.velocity.x = -a.velocity.x * params.collisionDamping;
  }
  // Y-axis collision
  if (a.position.y < minBound.y)
  {
    a.position.y = minBound.y;
    a.velocity.y = -a.velocity.y * params.collisionDamping;
  }
  else if (a.position.y > maxBound.y)
  {
    a.position.y = maxBound.y;
    a.velocity.y = -a.velocity.y * params.collisionDamping;
  }
}

void Fluid::randomInit(int n)
{
  std::srand(std::time(nullptr));
  for (int i = 0; i < n; i++)
  {
    float x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * boundSize.x;
    float y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * boundSize.y;
    particles[i] =
        Particle(
            x, y);
  }
}

void Fluid::gridInit(int cols, float gap)
{
  float xSpace = (cols - 1) * gap;
  int n = particles.size();
  int nRows = std::ceil(static_cast<float>(n) / static_cast<float>(cols));
  float ySpace = (nRows - 1) * gap;

  const float xStart = (boundSize.x / 2) - (xSpace / 2);
  float x = xStart;
  float y = (boundSize.y / 2) + (ySpace / 2);
  int created = 0;
  for (int r = 0; r < nRows && created < n; r++, y -= gap)
  {
    for (int c = 0; c < cols && created < n; c++, x += gap, created++)
    {
      particles[created] = Particle(x, y);
    }
    x = xStart;
  }
}

void Fluid::applyViscosity(float deltaTime)
{
  for (auto &i : particles)
  {
    for (auto &j : particles)
    {
      Vec rij = i.position - j.position;
      float q = rij.mag() / params.smoothingRadius;
      if (&i == &j)
        continue;
      if (q < 1.f)
      {
        // inward velocity
        float u = (i.velocity - j.velocity).dot(rij / rij.mag());
        if (u > 0)
        {
          Vec impulse = deltaTime * (1 - q) * (params.viscosityDelta * u + params.viscosityBeta * std::pow(u, 2)) * rij / rij.mag();
          i.velocity -= impulse;
          j.velocity += impulse;
        }
      }
    }
  }
}

void Fluid::computeDensityGrid(std::vector<std::vector<float>> &grid) const
{
  // expect zeroed vector of size renderWidth, renderWidth
  const int pxRadius = params.smoothingRadius * scale;
  for (const auto &p : particles)
  {
    Vec px = stor(p.position);
    for (int row = px.y - pxRadius; row <= px.y + pxRadius; row++)
    {
      for (int col = px.x - pxRadius; col <= px.x + pxRadius; col++)
      {
        if (row < 0 || row >= renderHeight || col < 0 || col >= renderWidth)
        {
          continue;
        }
        Vec rp = rtos(Vec(col, row));
        float dist = (rp - p.position).mag();
        grid[row][col] += smoothingKernel(dist);
      }
    }
  }
  for (auto &row : grid)
  {
    for (int i = 0; i < row.size(); ++i)
    {
      row[i] -= params.targetDensity;
    }
  }
  normalizeDensityGrid(grid);
}

void Fluid::normalizeDensityGrid(std::vector<std::vector<float>> &grid) const
{
  float min = std::numeric_limits<float>::max();
  float max = std::numeric_limits<float>::min();
  for (const auto &v : grid)
  {
    auto [rowMin, rowMax] = std::minmax_element(v.begin(), v.end());
    min = std::min(min, *rowMin);
    max = std::max(max, *rowMax);
    max = std::max(max, std::abs(min));
  }
  for (auto &row : grid)
  {
    for (int i = 0; i < row.size(); i++)
    {
      row[i] /= max;
    }
  }
}

int Fluid::getGridCell(const Vec &p) const
{
  int x = p.x / params.smoothingRadius;
  int y = p.y / params.smoothingRadius;
  return ((x * 73856093) ^ (y * 19349663)) % particles.size();
}

std::vector<std::pair<Particle *, int>> Fluid::gridParticles() const
{
  std::vector<std::pair<Particle *, int>> pairs(particles.size());
  std::transform(particles.begin(), particles.end(), pairs.begin(),
                 [&](const Particle &p)
                     -> std::pair<Particle *, int>
                 {
                   return {const_cast<Particle *>(&p), this->getGridCell(p.position)};
                 });
  return pairs;
}

void Fluid::sortGridParticles(
    std::vector<std::pair<Particle *, int>> &p)
{
  std::sort(p.begin(), p.end(), [](const auto &a, const auto &b)
            { return a.second < b.second; });
}

std::unordered_map<int, int> Fluid::mapGridStarts(
    std::vector<std::pair<Particle *, int>> &p)
    const
{
  std::unordered_map<int, int> gridStarts;
  std::for_each(p.begin(), p.end(), [&gridStarts, lastGrid=-1, i=0](const auto &pair) 
    mutable {
      if (pair.second != lastGrid){
        lastGrid = pair.second;
        gridStarts.insert({pair.second, i});
      }
      i++;
    }
  );
  return gridStarts;
}

void Fluid::sortParticlesIntoGrid() {
  std::vector<std::pair<Particle *, int>> gridded = this->gridParticles(); 
  sortGridParticles(gridded);
  this->gridStartIndices = mapGridStarts(gridded);
}