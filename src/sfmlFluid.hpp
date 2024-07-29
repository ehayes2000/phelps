#pragma once
#include <SFML/Graphics.hpp>
#include "fluid.hpp"

#define WIN_SIZE 512

class SfmlFluid {
private:
  Fluid fluid;
  sf::RenderWindow win;
void draw();
public:
  SfmlFluid(): 
    fluid(Fluid(WIN_SIZE, 0, 0.0000001, 0.2)), 
    win(sf::RenderWindow(sf::VideoMode(WIN_SIZE, WIN_SIZE), "phelps")){}
  void startRenderLoop();
};