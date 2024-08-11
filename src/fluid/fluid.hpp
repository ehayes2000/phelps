#pragma once

#include "vec.hpp"
#include "particles.hpp"
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
  const GridView& getGrid() const { return grid; }
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
  const Particles& getParticles() const { return particles; }

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
    for (int i = 0; i < particles.size; ++i){
      totalDensity += computeDensity(particles.positions[i]);
    }
    return totalDensity / particles.size;
  }
private:
  float scale;
  Particles particles;
  Vec boundSize;
  GridView grid;

  float smoothingKernel(const float dist) const;
  float smoothingNearKernel(const float dist) const;
  float computeNearDensity(const Vec &) const;
  float computeDensity(const Vec &) const;
  float computeNearPseudoPressure(const float) const;
  float computePseudoPressure(const float) const;
  void gridInit(int cols, float gap);
  void randomInit(int n);
  void boundryCollision(const int);
  void normalizeDensityGrid(std::vector<std::vector<float>>&) const;
  Vec relaxationDisplacement(
    const int i, 
    const int j,
    const float deltaTime,
    const float pressure,
    const float nearPressure) const;
  void doubleDensityRelaxation(float deltaTime);
  void applyViscosity(float deltaTime);
};
