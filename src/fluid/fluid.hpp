#pragma once

#include "vec.hpp"
#include "particle.hpp"
#include "gridView.hpp"
#include "adjacentParticles.hpp"
#include "fluidParams.hpp"

#include <numeric>
#include <cmath>
#include <vector> 
#include <ostream>
#include <iostream>
#include <map>


class Fluid { 
public:
  using Particles = std::vector<Particle>;
public:
  FluidParameters &params;
  Fluid(FluidParameters &params):
    scale(std::max(params.renderHeight, params.renderWidth)), 
    params(params),
    boundSize(params.renderWidth / scale, params.renderHeight / scale),
    grid(boundSize, params.smoothingRadius),
    particles(params.nParticles)
  {
    // randomInit(nParticles);
    gridInit(25, .04);
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
      scale * (p.x - params.renderRadius),
      scale * (boundSize.y - p.y - params.renderRadius) // 720 * (.666666 - .661 - .005)
    );
  }

  float computeAvgDensity() const {
    float totalDensity = 0.0f;
    for (auto &p: particles){ 
      totalDensity += computeDensity(p);
    }
    return totalDensity / particles.size();
  }
private:
  float scale;
  Particles particles;
  Vec boundSize;
  GridView grid;

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
