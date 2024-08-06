#pragma once

#include "vec.hpp"
#include "particle.hpp"
#include "gridView.hpp"
#include "adjacentParticles.hpp"

#include <numeric>
#include <cmath>
#include <vector> 
#include <ostream>
#include <iostream>
#include <map>


 
const float GRAVITY = 1.f;
const int NCOLS = 25;

class FluidParameters { 
public: 
  const float collisionDamping = .0f;
  const float smoothingRadius = .04; //.08
  const float particleMass = 1.f;
  const bool isGravity = false;
  const float targetDensity = 5.;
  const float pressureMultiplier = 0.0005f;
  const float nearPressureMultiplier = 0.01f;
  const float smoothingVolume = 2 * M_PI * smoothingRadius / 3.0;
  const float smoothingNearVolume = 2 * M_PI * smoothingRadius / 3.0;
  const float viscosityBeta = 0.9; // non-zero for viscos fluids
  const float viscosityDelta = .0f;
  FluidParameters() = default;
};


class Fluid { 
public:
  using Particles = std::vector<Particle>;
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
    grid(boundSize, params.smoothingRadius),
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
private:
  Particles particles;
  Vec boundSize;
  GridView grid;
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
  void normalizeDensityGrid(std::vector<std::vector<float>>&) const;
  Vec relaxationDisplacement(
    const Particle &, 
    const Particle &,
    const float deltaTime,
    const float pressure,
    const float nearPressure) const;
  void doubleDensityRelaxation(float deltaTime);
  void applyViscosity(float deltaTime);
};
