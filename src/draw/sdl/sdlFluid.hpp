#pragma once
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "../../fluid/fluid.hpp"
#include "../../fluid/fluidParams.hpp"
#include "../renderParams.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <stdio.h>
#include <SDL.h>

class Color { 
  public:
  Color() = default;
  Color(uint8_t r, uint8_t g, uint8_t b):
    r(r), 
    g(g),
    b(b)
    {}
  uint8_t r;
  uint8_t g;
  uint8_t b;
}; 



class SdlFluid { 
public:
  SdlFluid(FluidParameters params):
    fluidParams(params),
    fluid(fluidParams)
  { 
    init();
  }

  void startRenderLoop();
  static void stepMainLoop(SdlFluid *sdlFluid);
  
private:
  void init();
  FluidParameters fluidParams;
  RenderParameters params;
  Fluid fluid;
  SDL_Window* window;
  SDL_Renderer* renderer;
  void handleSdlEvent(const SDL_Event &event);
  void drawParticles(const float);
  void drawParticle(const int, const float);
  void highlightAdjacentParticles();
  void showDebugUi();
  Color plasmaGradient(float value, float minVal, float maxVal) const;

  Uint32 time;
  float deltaTime;
  bool done;
};