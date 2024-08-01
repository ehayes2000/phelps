#pragma once
#include <numeric>
#include <cmath>
#include <vector> 
#include <ostream>
#include <iostream>
#include "vec.hpp"

#define GRAVITY 32.f
#define PGAP 5
#define PCOLS 20

class FluidParameters { 
public: 
  const float collisionDamping = .7f;
  const float smoothingRadius = .1f;
  const float particleMass = .01f;
  const bool isGravity = false;
  const float targetDensity = 6.f;
  const float pressureMultiplier = .0005f;
  const float smoothingVolume = M_PI * std::pow(smoothingRadius, 8) / 4;
  FluidParameters() = default;
};


class Particle { 
public:
  Vec pos;
  Vec vel;
  Vec force;
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
  void computeAllDensities();
  float computeDensity(const Vec &p) const;
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
  const std::vector<Particle>& getParticles() const { return particles; }
  const std::vector<float>& getDensities() const { return densities; }
  const std::vector<Vec>& getPressures() const { return pressures; }
  void step(float deltaTime);
  void applyForce(Vec &p, float force, float radius);
  float computeAvgDensity(){
    float density = std::accumulate(densities.begin(), densities.end(), 0);
    return density / static_cast<float>(densities.size());
  }
};
