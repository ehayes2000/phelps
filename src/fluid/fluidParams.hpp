#pragma once
#include <cmath>
#include "vec.hpp"

struct FluidParameters { 
  float collisionDamping = .5f;
  float smoothingRadius = .08; //.08
  float renderRadius = .005;
  float particleMass = 1.f;
  float targetDensity = 5.;
  float pressureMultiplier = 0.0005f;
  float nearPressureMultiplier = 0.01f;
  float viscosityBeta = 0.0; // non-zero for viscos fluids
  float viscosityDelta = .5f;
  float gravity = 1.f;
  int renderHeight = 480;
  int renderWidth = 720;
  int nParticles = 100;
};