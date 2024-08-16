#include "fluid.hpp"
#include <ranges>

void Fluid::step(float deltaSec)
{
  grid.gridify(particles);
  if (deltaSec <= 0.f)
    return;
  applyViscosity(deltaSec);
  
  for (int i = 0; i < particles.size; ++ i)
  {
    particles.prevPositions[i] = particles.positions[i];
    particles.positions[i] += deltaSec * particles.velocities[i];
  }
  doubleDensityRelaxation();
  for (int i = 0; i < particles.size; ++i)
  {
    particles.velocities[i] = (particles.positions[i] - particles.prevPositions[i]) / deltaSec;
    boundryCollision(i);
  }
  for (auto &v : particles.velocities)
  {
    v += Vec(0.f, -deltaSec * params.gravity);
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

inline float Fluid::smoothingNearKernel(const float dist) const
{
  return nearKernel(dist, params.smoothingRadius);
}

inline float Fluid::computeNearDensity(const Vec &p) const
{
  float density = 0.f;
  for (const auto &j : grid.adj(p))
  {
    float dist = (p - particles.positions[j]).mag();
    if (dist > 0) {
      density += smoothingNearKernel(dist);
    }
  }
  return density;
}

inline float Fluid::smoothingKernel(const float dist) const
{
  return kernel(dist, params.smoothingRadius);
}

// TODO adjacent index!
inline float Fluid::computeDensity(const Vec &p) const
{
  float density = 0.f;
  for (const auto &j : grid.adj(p))
  {
    float dist = (p - particles.positions[j]).mag();
    if (dist > 0) 
    {
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

Vec Fluid::relaxationDisplacement(const int i,
                                  const int j,
                                  const float pressure,
                                  const float nearPressure) const
{
  Vec rij = particles.positions[i] - particles.positions[j];
  if (rij.mag() == 0.f)
  {
    return Vec(0, 0);
  }
  Vec rijUnit = rij / rij.mag();
  return rijUnit * (pressure * (1.f - rij.mag() / params.smoothingRadius) +
                    nearPressure * std::pow(1.f - rij.mag() / params.smoothingRadius, 2));
}

void Fluid::doubleDensityRelaxation()
{
  std::vector<Vec> npos(particles.size, Vec(0,0));
  std::vector<Vec> jnpos(particles.size, Vec(0,0));
  for (int i = 0; i < particles.size; i ++)
  {
    // Vec predicted = particles.positions[i] + deltaTime * particles.velocities[i];
    float density = computeDensity(particles.positions[i]);
    float pressure = computePseudoPressure(density);
    float nearDensity = computeNearDensity(particles.positions[i]);
    float nearPressure = computeNearPseudoPressure(nearDensity);
    Vec dx(0, 0);
    for (const auto &j : grid.adj(particles.positions[i]))
    {
      float q = (particles.positions[i] - particles.positions[j]).mag() / params.smoothingRadius;
      if(q > 0 && q < 1.f)
      {
        Vec displacement = relaxationDisplacement(i, j, pressure, nearPressure);
        // jnpos[j] -= displacement / 2.f;
        particles.positions[j] -= displacement / 2.f;
        dx += displacement / 2.f;
        dx += displacement;
      }
    }
    // npos[i] += dx;
    particles.positions[i] += dx;
  }
  // for (int i = 0; i < particles.size; ++i) { 
  //   particles.positions[i] += npos[i] + jnpos[i];
  // }
}

void Fluid::applyForce(Vec &p, float force, float radius)
{
  for (auto i: grid.adj(p)) { 
    Vec offset = particles.positions[i] - p;
    float dist = offset.mag();
    if (dist > 0) { 
      float influence = kernel(dist, radius);
      Vec forceVec = (offset / dist) * influence * force;
      particles.velocities[i] += forceVec;
    }
  }
}

void Fluid::boundryCollision(const int i)
{
  Vec minBound = Vec(0, 0) + Vec(params.renderRadius, params.renderRadius);
  Vec maxBound = boundSize - Vec(params.renderRadius, params.renderRadius);
  // X-axis collision
  Vec &pos = particles.positions[i];
  Vec &vel = particles.velocities[i];
  if (pos.x < minBound.x)
  {
    pos.x = minBound.x;
    vel.x = -vel.x * params.collisionDamping;
  }
  else if (pos.x > maxBound.x)
  {
    pos.x = maxBound.x;
    vel.x = -vel.x * params.collisionDamping;
  }
  // Y-axis collision
  if (pos.y < minBound.y)
  {
    pos.y = minBound.y;
    vel.y = -vel.y * params.collisionDamping;
  }
  else if (pos.y > maxBound.y)
  {
    pos.y = maxBound.y;
    vel.y = -vel.y * params.collisionDamping;
  }
}

void Fluid::randomInit(int n)
{
  std::srand(std::time(nullptr));
  for (int i = 0; i < n; i++)
  {
    float x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * boundSize.x;
    float y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * boundSize.y;
    particles.positions[i] = Vec(x, y);
    particles.prevPositions[i] = Vec(x, y);
    particles.velocities[i] = Vec(0, 0);
  }
}

void Fluid::gridInit(int cols, float gap)
{
  float xSpace = (cols - 1) * gap;
  int n = particles.size;
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
      particles.positions[created] = Vec(x, y);
      particles.prevPositions[created] = Vec(x, y);
      particles.velocities[created] = Vec(0, 0);
    }
    x = xStart;
  }
}

void Fluid::applyViscosity(float deltaTime)
{
  for (int i = 0; i < particles.size; ++i)
  {
    for (const auto &j : grid.adj(particles.positions[i]))
    {
      Vec rij = particles.positions[i] - particles.positions[j];
      float q = rij.mag() / params.smoothingRadius;
      if (q <= 0 || q > 1.f)
        return;
      // inward velocity
      float u = (particles.velocities[i] - particles.velocities[j]).dot(rij / rij.mag());
      if (u > 0)
      {
        Vec impulse = deltaTime * (1 - q) * (params.viscosityDelta * u + params.viscosityBeta * std::pow(u, 2)) * rij / rij.mag();
        particles.velocities[i] -= impulse;
        particles.velocities[j] += impulse;
      }
    }
  }
}

void Fluid::computeDensityGrid(std::vector<std::vector<float>> &grid) const
{
  grid[0][0] = 0;
  // // expect zeroed vector of size renderWidth, renderWidth
  // const int pxRadius = params.smoothingRadius * scale;
  // for (const auto &p : particles)
  // {
  //   Vec px = stor(p.position);
  //   for (int row = px.y - pxRadius; row <= px.y + pxRadius; row++)
  //   {
  //     for (int col = px.x - pxRadius; col <= px.x + pxRadius; col++)
  //     {
  //       if (row < 0 || row >= params.renderHeight || col < 0 || col >= params.renderWidth)
  //       {
  //         continue;
  //       }
  //       Vec rp = rtos(Vec(col, row));
  //       float dist = (rp - p.position).mag();
  //       grid[row][col] += smoothingKernel(dist);
  //     }
  //   }
  // }
  // for (auto &row : grid)
  // {
  //   for (int i = 0; i < row.size(); ++i)
  //   {
  //     row[i] -= params.targetDensity;
  //   }
  // }
  // normalizeDensityGrid(grid);
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
    for (int i = 0; i < static_cast<int>(row.size()); i++)
    {
      row[i] /= max;
    }
  }
}
