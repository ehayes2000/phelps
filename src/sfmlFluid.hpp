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
    scale(SfmlFluid::computeScale(Vec(width, height))),
    fluid(static_cast<float>(width) / computeScale(Vec(width, height)), 
          static_cast<float>(height) / computeScale(Vec(width, height)), 
          N_PARTICLES, 
          .01
    )
  {
    makeDrawables();
  }
  ~SfmlFluid(){ 
    for (const auto &p: drawables)
      delete p;
  }
  void startRenderLoop();
  static float computeScale(const Vec &s) {return std::max(s.x, s.y);}
};