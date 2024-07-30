#pragma once
#include <vector>
#include <iostream>

#define GRAVITY 32.174f

class Point { 
public:
  float x;
  float y;
  Point(): x(0), y(0){}
  Point(float x, float y):x(x), y(y){}
};

class Particle { 
public:
  Point pos;
  Point vel;
  Particle(): pos(-1, -1), vel(0, 0){}
  Particle(float x, float y, float vx, float vy): 
    pos(x, y),
    vel(vx, vy)
  {}
};

class Fluid { 
private:
  float width;
  float height;
  void initializeParticles();
public:
  std::vector<Particle> particles;
  Fluid(float width, float height, int nParticles): 
    particles(nParticles), 
    width(width), 
    height(height)
  {
    this->initializeParticles();
  }
  void step(float deltaTime);
};