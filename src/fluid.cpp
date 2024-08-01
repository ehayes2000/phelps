#include "fluid.hpp"

void Fluid::step(float deltaTime){
  Vec gravity(0, deltaTime * GRAVITY);
  computeAllDensities();
  for (int i = 0; i < particles.size(); i ++){
    particles[i].vel += particles[i].force * deltaTime;
    particles[i].pos += particles[i].vel * deltaTime;
    particles[i].force = Vec(0,0);
    // particles[i].force += 
    Vec pressure = computePressureForce(particles[i].pos);
    pressure /= densities[i] != 0.f ? densities[i] : 1.f;
    particles[i].force += pressure;
    boundryCollision(particles[i]);
    if (params.isGravity){
      particles[i].vel -= gravity;
    }
  }
  std::cout << computeAvgDensity() << std::endl;
}

void Fluid::gridInit(int cols, int n, float gap){
  float xSpace = (cols - 1) * gap;
  int nRows = std::ceil(static_cast<float>(n) / static_cast<float>(cols));
  float ySpace = (nRows - 1) * gap;
  const float xStart = (boundSize.x / 2) - (xSpace / 2);
  float x = xStart;
  float y = (boundSize.y / 2) + (ySpace / 2);
  int created = 0;
  for (int r = 0; r < nRows && created < n; r++, y -= gap) { 
    for (int c = 0; c < cols && created < n; c++, x += gap, created ++){
      particles[created] = Particle(x, y, 0, 0);
    }
    x = xStart;
  }
}

void Fluid::boundryCollision(Particle &a){
  Vec minBound = Vec(0, 0) + Vec(radius, radius);
  Vec maxBound = boundSize - Vec(radius, radius);

  // X-axis collision
  if (a.pos.x < minBound.x) {
      a.pos.x = minBound.x;
      a.vel.x = -a.vel.x * params.collisionDamping;
  } else if (a.pos.x > maxBound.x) {
      a.pos.x = maxBound.x;
      a.vel.x = -a.vel.x * params.collisionDamping;
  }
  // Y-axis collision
  if (a.pos.y < minBound.y) {
      a.pos.y = minBound.y;
      a.vel.y = -a.vel.y * params.collisionDamping;
  } else if (a.pos.y > maxBound.y) {
      a.pos.y = maxBound.y;
      a.vel.y = -a.vel.y * params.collisionDamping;
  }
}

float Fluid::smoothingKernel(const float &radius, const float &dst) const {
  if (dst >= radius)
    return 0;
  double volume = (M_PI * std::pow(radius, 4)) / 6.f;
  return std::pow(radius - dst, 2) / volume;
}

float Fluid::smoothingKernelDerivative(const float &radius, const float &dst) const {
  if (dst >= radius) return 0;
  float scale = 12.f / (std::pow(radius, 4) * M_PI);
  return (dst - radius) * scale;
}

float Fluid::computeDensity(const Vec &p) const { 
  float density = .000001f;
  for (const auto &x : particles){ 
    float dst = (x.pos - p).mag();
    float influence = smoothingKernel(params.smoothingRadius, dst);
    density += params.particleMass * influence;
  }
  return density;
}

void Fluid::computeAllDensities() { 
  for (int i = 0; i < particles.size(); i ++){
    densities[i] = computeDensity(particles[i].pos);
  }
}

void Fluid::randomInit(int n){ 
  std::srand(std::time(nullptr));
  for (int i = 0; i < n; i ++){
    float x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * boundSize.x;
    float y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * boundSize.y;
    particles[i] = 
      Particle(
        x, y,
        0, 0
      );
  }
}
float Fluid::densityToPressure(const float &density) const { 
  float err = density - params.targetDensity;
  return err * params.pressureMultiplier;
}

const Vec Fluid::computePressureForce(const Vec& p){
  Vec pressure = Vec(0, 0);
  for (int i = 0; i < particles.size(); i ++){
      if (&p == &particles[i].pos)
        continue;
      float dst = (particles[i].pos - p).mag();
      Vec dir = Vec(particles[i].pos - p) / dst;
      float slope = smoothingKernelDerivative(params.smoothingRadius, dst);
      float density = densities[i];
      pressure += densityToPressure(density) * dir * slope / density;
  }
  return pressure;
}

void Fluid::applyForce(Vec &p, float force, float radius){ 
  for (auto &particle : particles) { 
    Vec offset = particle.pos - p;
    float dist = offset.mag();
    if (dist < radius){
      float influence = smoothingKernel(radius, dist);
      Vec force = (offset / dist) * influence; 
      particle.force += force;
      
    }
  }
}