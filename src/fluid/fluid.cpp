#include "fluid.hpp"
#include <ranges>
#include <algorithm>
#include <ctime>

void Fluid::step(float deltaSec)
{
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
  // particleCollision();
  for (auto &v : particles.velocities)
  {
    v += Vec(0.f, -deltaSec * params.gravity);
  }
  grid.gridify(particles);
}

float kernel(const float dist, const float radius)
{
  if (dist > radius)
    return 0.f;
  float k = std::min(std::pow(1 - dist / radius, 2), .1);
  return k;
}

float cubicKernel(const float dist, const float radius) { 
  float v = radius * radius - dist * dist;
	return v * v * v; 
}

float nearKernel(const float dist, const float radius)
{
  if (dist > radius)
    return 0.f;
  // return std::min(std::pow(1 - dist / radius, 3), .05);
  return 0;
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

void Fluid::pushForce(Vec &p, float radius)
{
  for (int i = 0; i < particles.size; ++i){
    Vec offset = particles.positions[i] - p;
    float dist = offset.mag();
    if (dist > 0 && dist < params.forceRadius){
      float influence = cubicKernel(dist, radius) * 1000;
      Vec forceVec = (offset / dist) * influence;
      particles.velocities[i] += forceVec;
    }
  }
}

void Fluid::pullForce(Vec &p) { 
  for (int i = 0; i < particles.size; ++i){
    Vec offset = particles.positions[i] - p;
    float dist = offset.mag();
    if (dist > 0 && dist < params.forceRadius){
      float influence = -.01;
      Vec forceVec = (offset / dist) * influence;
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
    int n = particles.size;
    int rows = (n + cols - 1) / cols;  // Ceiling division to get number of rows

    float totalWidth = (cols - 1) * gap;
    float totalHeight = (rows - 1) * gap;
    
    float startX = (boundSize.x - totalWidth) / 2;
    float startY = (boundSize.y - totalHeight) / 2;

    for (int i = 0; i < n; i++)
    {
        int row = i / cols;
        int col = i % cols;

        float x = startX + col * gap;
        float y = startY + row * gap;

        particles.positions[i] = Vec(x, y);
        particles.prevPositions[i] = Vec(x, y);
        particles.velocities[i] = Vec(0, 0);
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

void Fluid::particleCollision() { 
  for (int _ = 0; _ < params.resolutionIterations; ++_) { 
    for (int i = 0; i < particles.size; ++ i) { 
      for (int j : grid.adj(particles.positions[i])) { 
        Vec ij = particles.positions[i] - particles.positions[j];
        float ijMag = ij.mag(); 
        if (ijMag == 0) continue;
        if (ijMag < 2 * params.renderRadius){ 
          Vec correction = (2 * params.renderRadius - ijMag) * ij / ijMag;
          particles.positions[i] += 0.5 * correction;
          particles.positions[j] -= 0.5 * correction;
        }
      }
    }
  }
}

void Fluid::fullGridInit() {
  int i = 0;
  std::vector<Vec> pos;
  std::vector<Vec> prevPos;
  for (float xp = .005; xp <= 1; xp += params.smoothingRadius / 1.3){  
    for (float yp = .01; yp <= boundSize.y - .01 ; yp += params.smoothingRadius / 1.3, ++i) { 
      pos.push_back(Vec(xp, yp));
      prevPos.push_back(Vec(xp, yp));
    }
  }
  std::vector<Vec> vel(pos.size(), Vec(0, 0));
  this->particles = Particles(pos, prevPos, vel);
}

void Fluid::resize(int width, int height) { 
  scale = std::max(width, height);
  boundSize = Vec(static_cast<float>(width) / scale, static_cast<float>(height) / scale);
}
