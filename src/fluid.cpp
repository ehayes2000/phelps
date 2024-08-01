#include "fluid.hpp"

void Fluid::step(float deltaTime){
  if (params.isGravity){
    for (auto &p: particles){
      p.vel.y -= deltaTime * GRAVITY;
    }
  }
  computeAllDensities();
  for (int i = 0; i < particles.size(); i ++){

    particles[i].pos += particles[i].vel * deltaTime;
    boundryCollision(particles[i]);
    Vec pressureForce = computePressureForce(particles[i].pos);
    Vec acceleration = pressureForce / densities[i];
    // std::cout << "pressure: " << pressureForce << ", acceleration: " << acceleration << std::endl;
    particles[i].vel += acceleration * deltaTime;
  }
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

void Fluid::collide(Particle &a, const Particle &b){
  if (a.dist(b) > radius)
    return;
  
}

void Fluid::boundryCollision(Particle &a){
  if (a.pos.y - radius <= 0 || a.pos.y + radius >= boundSize.y){
    a.vel.y *= -1.f * params.collisionDamping;
  }
  if (a.pos.x - radius <= 0 || a.pos.x + radius >= boundSize.x){
    a.vel.x *= -1.f * params.collisionDamping;
  }
}

float Fluid::smoothingKernel(const float &radius, const float &dst) const {
  double v = std::max(0.0, std::pow(radius, 2) - std::pow(dst, 2));
  return std::pow(v, 3) / params.smoothingVolume;
}

float Fluid::smoothingKernelDerivative(const float &radius, const float &dst) const {
  if (dst >= radius) return 0;
  // return -6.f * dst * std::pow(dst*dst - radius*radius, 2);
  float f = radius * radius - dst * dst;
  float scale = -24 / (M_PI * std::pow(radius, 8));
  return scale * dst * f * f;
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

const std::vector<float>& Fluid::computeAllDensities() { 
  for (int i = 0; i < particles.size(); i ++){
    densities[i] = computeDensity(particles[i].pos);
  }
  return densities;
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
      pressure += -densityToPressure(density) * dir * slope / density;
  }
  return pressure;
}

// const std::vector<Vec>& Fluid::computeDensityGradient(){
//   computeAllDensities();  
//   for (int j = 0; j < particles.size(); j ++){
//     Vec grad = Vec(0, 0);
//     for (int i = 0; i < particles.size(); i ++){
//       if (i == j)
//         continue;
//       float dst = (particles[i].pos - particles[j].pos).mag();
//       Vec dir = Vec(particles[i].pos - particles[j].pos) / dst;
//       float slope = smoothingKernelDerivative(params.smoothingRadius, dst);
//       float density = densities[i];
//       grad += -densities[i] * dir * slope / density;
//     }
//     densityGradient[j] = grad;
//   }
//   std::cout << "gradiented\n";
//   return densityGradient;
// }