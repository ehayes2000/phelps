#pragma once
#include <SFML/Graphics.hpp>
#include "../fluid/fluid.hpp"

struct SfParams { 
  bool isPaused = false;
  bool isDebugMenu = false;
  bool isDensityView = false;
  bool isLClick = false;
  bool isRClick = false;
};

class SfmlFluid { 
public:
  SfmlFluid(FluidParameters params):
    window(sf::VideoMode(params.renderWidth, params.renderHeight), "Phelps"),
    fluid(params),
    height(params.renderHeight),
    width(params.renderWidth),
    fluidParams(params)
  {
  }
  ~SfmlFluid()=default;
  void startRenderLoop();
private:
  Fluid fluid;
  int height;
  int width;
  sf::RenderWindow window;
  FluidParameters fluidParams;
  SfParams params;
  const sf::Sprite getDensityImage() const;
  sf::CircleShape makeDrawable(const Particle &p) const ;
  sf::Color plasmaGradient(float value, float minVal, float maxVal) const;
  void handleSfEvent(sf::Event&);
  void drawParticles();

};