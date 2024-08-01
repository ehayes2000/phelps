#pragma once
#include <cmath>
#include <vector> 
#include <ostream>
#include <iostream>
#include "vec.hpp"

#define GRAVITY 32.174f
#define PGAP 5
#define PCOLS 20

class FluidParameters { 
public: 
  const float collisionDamping = .9f;
  const float smoothingRadius = .05f;
  const float particleMass = 1.f;
  const bool isGravity = false;
  const float targetDensity = 2.75f;
  const float pressureMultiplier = .015;
  const float smoothingVolume = M_PI * std::pow(smoothingRadius, 8) / 4;
  FluidParameters() = default;
};


class Particle { 
public:
  Vec pos;
  Vec vel;
  Particle(): pos(-1, -1), vel(0, 0){}
  Particle(float x, float y, float vx, float vy): 
    pos(x, y),
    vel(vx, vy)
  {}
  const float dist(const Particle &pt){
    return std::sqrt(
      std::pow(pos.x - pt.pos.x, 2) + 
      std::pow(pos.y - pt.pos.y, 2)
    );
  }
};

class Fluid { 
private:
  std::vector<Particle> particles;
  std::vector<float> densities;
  std::vector<Vec> pressures;
  void initializeParticles();
  void gridInit(int cols, int n, float gap);
  void randomInit(int n);
  void collide(Particle &a, const Particle &b);
  void boundryCollision(Particle &a);
  float smoothingKernel(const float &radius, const float &dst) const;
  float smoothingKernelDerivative(const float &radius, const float &dst) const;
  float densityToPressure(const float &density) const ;
  const Vec computePressureForce(const Vec&);
public:
  const Vec boundSize;
  const float radius;
  FluidParameters params;
  Fluid(float width, float height, int nParticles, float radius): 
    particles(nParticles), 
    densities(nParticles),
    pressures(nParticles),
    boundSize(width, height),
    radius(radius)
  {
    randomInit(nParticles);
    // gridInit(PCOLS, nParticles, PGAP);
  }
  const std::vector<Particle>& getParticles() const { 
    return particles;
  }
  void step(float deltaTime);
  float computeDensity(const Vec &p) const;
  const std::vector<float>& computeAllDensities();
  // const std::vector<Vec>& computeDensityGradient();
};
