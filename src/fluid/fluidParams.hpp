#pragma once
#include <cmath>
#include "vec.hpp"

class FluidParameters { 
public:
  float collisionDamping = .5f;
  float smoothingRadius = .08; //.08
  float renderRadius = .005;
  float particleMass = 1.f;
  float targetDensity = 1.;
  float pressureMultiplier = 0.001f;
  float nearPressureMultiplier = 0.001f;
  float viscosityBeta = 0.0; // non-zero for viscos fluids
  float viscosityDelta = .5;
  float gravity = 0.f;
  int renderHeight = 720;
  int renderWidth = 1080;
  int nParticles = 700;
};
