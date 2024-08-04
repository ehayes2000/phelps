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
  const float collisionDamping = .1f;
  const float smoothingRadius = .1; //.08
  const float particleMass = 1.f;
  const bool isGravity = true;
  const float targetDensity = 3.;
  const float pressureMultiplier = 0.005f;
  const float nearPressureMultiplier = 0.001f;
  const float smoothingVolume = 2 * M_PI * smoothingRadius / 3.0;
  const float smoothingNearVolume = 2 * M_PI * smoothingRadius / 3.0;
  const float viscosityBeta = 0.0; // non-zero for viscos fluids
  const float viscosityDelta = .9f;
  FluidParameters() = default;
};


class Fluid { 
private:
  std::vector<Particle> particles;
  Vec boundSize;
  int renderHeight;
  int renderWidth;
  float scale;
  float smoothingKernel(const float dist) const;
  float smoothingNearKernel(const float dist) const;
  float computeNearDensity(const Particle &p) const;
  float computeDensity(const Particle &p) const;
  float computeNearPseudoPressure(const float) const;
  float computePseudoPressure(const float) const;
  void gridInit(int cols, float gap);
  void randomInit(int n);
  void boundryCollision(Particle &a);
  Vec relaxationDisplacement(
    const Particle &, 
    const Particle &,
    const float deltaTime,
    const float pressure,
    const float nearPressure) const;
  void doubleDensityRelaxation(float deltaTime);
  void applyViscosity(float deltaTime);
public:
  const float radius;
  FluidParameters params;
  Fluid(int width, 
        int height, 
        int nParticles, 
        float radius): 
    particles(nParticles), 
    scale(std::max(width, height)),
    boundSize(static_cast<float>(width)/static_cast<float>(std::max(width, height)),
      static_cast<float>(height)/ static_cast<float>(std::max(width, height))),
    renderHeight(height),
    renderWidth(width),
    radius(radius)
  {
    // randomInit(nParticles);
    gridInit(NCOLS, .04);
  }
  void step(float deltaTime);
  void applyForce(Vec &p, float force, float radius);
  void computeDensityGrid(std::vector<std::vector<float>> &) const;
  float getScale() const { return scale; }
  const std::vector<Particle>& getParticles() const { return particles; }

  Vec rtos(const Vec& p) const { 
    /*
      render to simulation space
    */ 
    return Vec(
      p.x / scale,
      boundSize.y - p.y / scale
    );
  }

  Vec stor(const Vec &p) const { 
    /*
      simulation to render space
    */
    return Vec(
      scale * (p.x - radius),
      scale * (boundSize.y - p.y - radius)
    );
  }

  float computeAvgDensity(){
    float totalDensity = 0.0f;
    for (auto &p: particles){ 
      totalDensity += computeDensity(p);
    }
    return totalDensity / particles.size();
  }
};
