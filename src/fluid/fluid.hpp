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
    params(params),
    scale(std::max(params.renderHeight, params.renderWidth)), 
    particles(params.nParticles),
    boundSize(params.renderWidth / scale, params.renderHeight / scale),
    grid(boundSize, params.smoothingRadius)
  {
    // randomInit(nParticles);
    // gridInit(200, params.smoothingRadius / 1.3);
    fullGridInit();
  }

  void setBounds(int h, int w){  
    params.renderWidth = w;
    params.renderHeight = h;
    scale = std::max(params.renderHeight, params.renderWidth);
    boundSize = Vec(params.renderWidth / scale, params.renderHeight / scale);
  }

  void step(float deltaSec);
  void pushForce(Vec &p);
  void pullForce(Vec &p);
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
      scale * (p.x),
      scale * (boundSize.y - p.y)
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
  void fullGridInit();
  void randomInit(int n);
  void boundryCollision(const int);
  void normalizeDensityGrid(std::vector<std::vector<float>>&) const;
  Vec relaxationDisplacement(
    const int i, 
    const int j,
    const float pressure,
    const float nearPressure) const;
  void doubleDensityRelaxation();
  void applyViscosity(float deltaTime);
  void particleCollision();
};
