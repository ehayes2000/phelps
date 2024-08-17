#pragma once
#include <cmath>
#include "vec.hpp"

class FluidParameters { 
public:
  float collisionDamping = .5f;
  float smoothingRadius = .015; //.08
  float renderRadius = .002;
  float targetDensity = .15;
  float pressureMultiplier = 0.002f;
  float nearPressureMultiplier = 0.000f;
  float viscosityBeta = 0.0; // non-zero for viscos fluids
  float viscosityDelta = .5;
  float gravity = 0.f;
  int renderHeight = 1107;
  int renderWidth = 1710;
  int nParticles = 8000;
};
