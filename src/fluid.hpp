#pragma once

#include "vec.hpp"
#include "particle.hpp"

#include <numeric>
#include <cmath>
#include <vector> 
#include <ostream>
#include <iostream>



const float GRAVITY = 5.f;
const int NCOLS = 25;

class FluidParameters { 
public: 
  const float collisionDamping = .4f;
  const float smoothingRadius = .06; //.08
  const float particleMass = 1.f;
  const bool isGravity = true;
  const float targetDensity = 9.f;
  const float pressureMultiplier = 100.5f;
  const float smoothingVolume = M_PI * std::pow(smoothingRadius, 8) / 4;
  const float viscosityBeta = .0f; // high for highly viscous fluids else 0
  const float viscosityDelta = 1.5f; 
  FluidParameters() = default;
};


class Fluid { 
private:
  std::vector<Particle> particles;
  void gridInit(int cols, float gap);
  void randomInit(int n);
  void boundryCollision(Particle &a);
  float smoothingKernel(const float dist) const;
  float computeDensity(const Particle &p) const;
  float computePseudoPressure(const float) const;
  Vec relaxationDisplacement(const float deltaTime, const float density, const Particle &, const Particle &) const;
  void doubleDensityRelaxation(float deltaTime);
  void applyViscosity(float deltaTime);
public:
  const Vec boundSize;
  const float radius;
  FluidParameters params;
  Fluid(float width, float height, int nParticles, float radius): 
    particles(nParticles), 
    boundSize(width, height),
    radius(radius)
  {
    // randomInit(nParticles);
    gridInit(NCOLS, .04);
}
  const std::vector<Particle>& getParticles() const { return particles; }
  void step(float deltaTime);
  void applyForce(Vec &p, float force, float radius);
  float computeAvgDensity(){
    float totalDensity = 0.0f;
    for (auto &p: particles){ 
      totalDensity += computeDensity(p);
    }
    return totalDensity / particles.size();
  }
};
