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

class Color
{
public:
  Color() = default;
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r),
                                           g(g),
                                           b(b)
  {
  }
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

class SdlFluid
{
public:
  SdlFluid(FluidParameters params) : fluidParams(params),
                                     fluid(fluidParams)
  {
    // Setup SDL
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE);
    this->window = SDL_CreateWindow("Phelps", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, fluidParams.renderWidth, fluidParams.renderHeight, window_flags);

    if (window == nullptr)
    {
      printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
      exit(1);
    }
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
      SDL_Log("Error creating SDL_Renderer!");
      exit(1);
    }

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // main loop vars :)
    done = false;
    fluid.step(.0001);
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
  }

  ~SdlFluid() { 
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();  
  }
  Fluid operator=(Fluid &f)
  {
    return f;
  }
  void startRenderLoop();
  void stepRenderLoop();
  void cleanupRenderLoop();
  bool isDone() const { return done; }

private:
  void init();
  static constexpr float ticksPerSecond = 1000.f;
  FluidParameters fluidParams;
  RenderParameters params;
  Fluid fluid;
  SDL_Window *window;
  SDL_Renderer *renderer;
  void handleSdlEvent(const SDL_Event &event);
  void drawParticles(const float);
  void drawParticle(const int, const float);
  void highlightAdjacentParticles();
  void showDebugUi();
  void reset();
  Color plasmaGradient(float value, float minVal, float maxVal) const;

  Uint32 nowTicks, prevTicks;
  bool done;
};