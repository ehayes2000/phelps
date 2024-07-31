#pragma once
#include <SFML/Graphics.hpp>
#include "fluid.hpp"

#define N_PARTICLES 300

class SfmlFluid { 
private:
  Fluid fluid;
  std::vector<sf::CircleShape*> drawables;
  std::vector<sf::CircleShape*> &makeDrawables();
  sf::CircleShape* makeDrawable(const Particle &p);
  Vec stor(const Vec &p);
  void updateDrawables();
  int winHeight;
  int winWidth;
  const float scale;
public:
  SfmlFluid(int width, int height): 
    winWidth(width),
    winHeight(height),
    scale(static_cast<float>(height)/100.f),
    fluid(150, 100, N_PARTICLES, 1)
  {
    makeDrawables();
  }
  ~SfmlFluid(){ 
    for (const auto &p: drawables)
      delete p;
  }
  void startRenderLoop();
};