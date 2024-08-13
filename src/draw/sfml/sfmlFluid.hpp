#pragma once
#include "renderParams.hpp"
#include "../fluid/fluid.hpp"
#include <SFML/Graphics.hpp>


class SfmlFluid { 
public:
  SfmlFluid(FluidParameters params):
    fluidParams(params),
    window(sf::VideoMode(fluidParams.renderWidth, fluidParams.renderHeight), "Phelps"),
    fluid(fluidParams),
    height(fluidParams.renderHeight),
    width(fluidParams.renderWidth)
  {}
  ~SfmlFluid()=default;
  void startRenderLoop();
private:
  FluidParameters fluidParams;
  RenderParameters params;
  Fluid fluid;
  int height;
  int width;
  sf::RenderWindow window;
  const sf::Sprite getDensityImage() const;
  sf::CircleShape makeDrawable(const int) const ;
  sf::Color plasmaGradient(float value, float minVal, float maxVal) const;
  void handleSfEvent(sf::Event&);
  void drawParticles();
  void highlightAdjacentParticles();
};