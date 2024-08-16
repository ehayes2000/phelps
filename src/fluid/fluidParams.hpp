#pragma once
#include <cmath>
#include "vec.hpp"

class FluidParameters { 
public:
  float collisionDamping = .5f;
  float smoothingRadius = .03; //.08
  float renderRadius = .002;
  float targetDensity = .5;
  float pressureMultiplier = 0.001f;
  float nearPressureMultiplier = 0.000f;
  float viscosityBeta = 0.0; // non-zero for viscos fluids
  float viscosityDelta = .5;
  float gravity = 0.f;
  int renderHeight = 1080;
  int renderWidth = 1920;
  int nParticles = 2000;
};
