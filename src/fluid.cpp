#include "fluid.hpp"

void Fluid::step(float deltaTime){
  for (auto &p: particles){
    p.pos.y += p.vel.y * deltaTime;
    p.vel.y -= deltaTime * GRAVITY;
  }
}

void Fluid::gridInit(int cols, int n, float gap){
  float xSpace = (cols - 1) * gap;
  int nRows = std::ceil(static_cast<float>(n) / static_cast<float>(cols));
  float ySpace = (nRows - 1) * gap;
  const float xStart = (width / 2) - (xSpace / 2);
  float x = xStart;
  float y = (height / 2) + (ySpace / 2);
  int created = 0;
  for (int r = 0; r < nRows && created < n; r++, y -= gap) { 
    for (int c = 0; c < cols && created < n; c++, x += gap, created ++){
      particles.push_back(Particle(x, y, 0, 0));
    }
    x = xStart;
  }
}

void Fluid::initializeParticles(){
  gridInit(10, 50, 5);
}