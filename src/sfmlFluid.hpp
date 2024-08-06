#pragma once
#include <SFML/Graphics.hpp>
#include "fluid.hpp"

const int N_PARTICLES = 1000;

class SfmlFluid { 
private:
  Fluid fluid;
  std::vector<sf::CircleShape*> drawables;
  std::vector<sf::CircleShape*> &makeDrawables();
  sf::CircleShape* makeDrawable(const Particle &p);
  sf::Color plasmaGradient(float value, float minVal, float maxVal) const;
  sf::RenderWindow window;
  void updateDrawables();
  void renderParticles();
  int height;
  int width;
public:
  SfmlFluid(int width, int height): 
    height(height),
    width(width),
    window(sf::VideoMode(width, height), "Phelps"),
    fluid(width, height,
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
};