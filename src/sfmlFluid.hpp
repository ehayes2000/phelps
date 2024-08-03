#pragma once
#include <SFML/Graphics.hpp>
#include "fluid.hpp"

const int N_PARTICLES= 400;

class SfmlFluid { 
private:
  Fluid fluid;
  std::vector<sf::CircleShape*> drawables;
  std::vector<sf::CircleShape*> &makeDrawables();
  sf::CircleShape* makeDrawable(const Particle &p);
  sf::Color plasmaGradient(float value, float minVal, float maxVal) const;
  sf::RenderWindow window;
  Vec stor(const Vec &p) const;
  Vec rtos(const Vec &p) const;
  void updateDrawables();
  void renderParticles();
  const float scale;
public:
  SfmlFluid(int width, int height): 
    scale(SfmlFluid::computeScale(Vec(width, height))),
    window(sf::VideoMode(width, height), "Phelps"),
    fluid(static_cast<float>(width) / computeScale(Vec(width, height)), 
          static_cast<float>(height) / computeScale(Vec(width, height)), 
          N_PARTICLES, 
          .005
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