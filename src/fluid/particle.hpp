#pragma once
#include "vec.hpp"

enum Attribute { 
  POSITION, 
  POSITION_PREV,
  VELOCITY,
  N_ATTRIBUTES
};

class Particle { 
public:
  Vec position;
  Vec positionPrev;
  Vec velocity;
  Particle() = default;
  Particle(Vec pos): position(pos) {}
  Particle(float x, float y): position(Vec(x, y)) {}
};