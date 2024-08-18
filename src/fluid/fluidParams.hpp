#pragma once
#include <cmath>
#include "vec.hpp"

class FluidParameters { 
public:
  float collisionDamping = .5f;
  float smoothingRadius = .0135; //.08
  float renderRadius = .0018;
  float targetDensity = .1;
  float pressureMultiplier = 0.002f;
  float nearPressureMultiplier = 0.000f;
  float viscosityBeta = 0.9; // non-zero for viscos fluids
  float viscosityDelta = .5;
  float gravity = 0.f;
  int renderHeight = 1000;
  int renderWidth = 1710;
  int nParticles = 8000;
  int resolutionIterations = 3;
  float forceRadius = .18;
};
