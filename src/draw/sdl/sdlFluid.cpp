#include "sdlFluid.hpp"
#include "circle.h"

void SdlFluid::drawParticles(const float deltaTime)
{
  for (int i = 0; i < fluid.getParticles().size; ++i)
  {
    drawParticle(i, deltaTime);
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void SdlFluid::handleSdlEvent(const SDL_Event &event)
{
  if (event.type == SDL_EventType::SDL_KEYDOWN && event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_ESCAPE)
  {
    params.isDebugMenu = !params.isDebugMenu;
  }
  else if (event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
  {
    params.isLClick = true;
  }
  else if (event.type == SDL_EventType::SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
  {
    params.isLClick = false;
  }
  else if (event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
  {
    params.isRClick = true;
  }
  else if (event.type == SDL_EventType::SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT)
  {
    params.isRClick = false;
  }
  else if (event.type == SDL_EventType::SDL_MOUSEMOTION)
  {
    params.mousePos.x = event.motion.x;
    params.mousePos.y = event.motion.y;
  }
  else if (event.type == SDL_EventType::SDL_KEYDOWN && event.key.keysym.scancode == SDL_Scancode::SDL_SCANCODE_SPACE){ 
    params.isPaused = !params.isPaused;
  }
}

void SdlFluid::init()
{

  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
  {
    printf("Error: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(NULL);
  this->window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, fluidParams.renderWidth, fluidParams.renderHeight, window_flags);

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

void SdlFluid::startRenderLoop()
{
  while (!done)
  {
    this->stepRenderLoop();
  }
  cleanupRenderLoop();
}

void SdlFluid::cleanupRenderLoop() { 
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void SdlFluid::showDebugUi()
{

  // const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
  // ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(550, 6), ImGuiCond_FirstUseEver);
  ImGuiWindowFlags window_flags = 0;
  bool p_open;
  if (!ImGui::Begin("Debug", &p_open, window_flags))
  {
    ImGui::End();
    return;
  }
  ImGui::Checkbox("Adjacent View", &params.isAdjacentView);
  ImGui::Separator();
  const float dragSpeed = .001;
  const float range = 100.f;
  const float smoothingRadius = fluidParams.smoothingRadius;
  ImGui::Text("Fluid Parameters");
  ImGui::DragFloat("Collision Damping", &fluidParams.collisionDamping, dragSpeed, -range, range);
  ImGui::DragFloat("Smoothing Radius", &fluidParams.smoothingRadius, dragSpeed, -range, range);
  ImGui::DragFloat("Target Density", &fluidParams.targetDensity, dragSpeed, -range, range);
  ImGui::DragFloat("Pressure", &fluidParams.pressureMultiplier, dragSpeed, -range, range);
  ImGui::DragFloat("Near Pressure", &fluidParams.nearPressureMultiplier, dragSpeed, -range, range);
  ImGui::DragFloat("Viscosity Beta", &fluidParams.viscosityBeta, dragSpeed, -range, range);
  ImGui::DragFloat("Viscosity Delta", &fluidParams.viscosityDelta, dragSpeed, -range, range);
  if (smoothingRadius != fluidParams.smoothingRadius){
    fluid.regrid();
  }
  ImGui::End();
}

void SdlFluid::stepRenderLoop()
{
  SDL_RenderClear(renderer);
  nowTicks = SDL_GetTicks(); 
  float deltaSec = static_cast<float>(nowTicks - prevTicks) / ticksPerSecond;
  prevTicks = nowTicks;

  SDL_Event event;

  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    handleSdlEvent(event);
    if (event.type == SDL_QUIT)
      done = true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
      done = true;
  }
  if (params.isDensityView)
  {
    // TODO
  }
  else if (params.isAdjacentView)
  {
    drawParticles(deltaSec);
    highlightAdjacentParticles();
  }
  else
  {
    drawParticles(deltaSec);
  }
  if (params.isDebugMenu)
  {
    showDebugUi();
  }
  else if (!params.isPaused)
  {
    fluid.step(deltaSec);
  }
  if (params.isLClick && !params.isPaused)
  {
    Vec simPoint = fluid.rtos(params.mousePos);
    fluid.pushForce(simPoint);
  }
  else if (params.isRClick && !params.isPaused)
  {
    Vec simPoint = fluid.rtos(params.mousePos);
    fluid.pullForce(simPoint);
  }
  ImGui::Render();
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
  SDL_RenderPresent(renderer);
}

void SdlFluid::drawParticle(const int i, const float deltaTime)
{
  const Particles &particles = fluid.getParticles();
  const Vec &pos = particles.positions[i];
  const Vec &vel = particles.velocities[i];
  Color c = plasmaGradient(vel.mag(), 0, .002 / deltaTime);
  Vec rpos = fluid.stor(pos);
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 0);
  SDL_RenderFillCircle(renderer, rpos.x, rpos.y, fluid.getScale() * fluidParams.renderRadius);
}

void SdlFluid::highlightAdjacentParticles()
{
  Vec simPos = fluid.rtos(params.mousePos);
  GridView grid = fluid.getGrid();
  for (int i : grid.adj(simPos)){
    Vec pos = fluid.stor(fluid.getParticles().positions[i]);
    SDL_SetRenderDrawColor(renderer, 0, 150, 0, 0);
    SDL_RenderFillCircle(renderer, pos.x, pos.y, fluid.getScale()* fluidParams.renderRadius);
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0 ,0);
}

Color SdlFluid::plasmaGradient(float value, float minValue, float maxValue) const
{
  // Normalize the value to a range between 0 and 1
  float t = (value - minValue) / (maxValue - minValue);
  t = std::clamp(t, 0.0f, 1.0f);

  // Define key colors for the plasma gradient
  const Color color1(13, 8, 135);   // Dark blue
  const Color color2(84, 2, 163);   // Purple
  const Color color3(139, 10, 165); // Magenta
  const Color color4(185, 50, 137); // Pink
  const Color color5(219, 92, 104); // Light red
  const Color color6(244, 143, 77); // Orange
  const Color color7(254, 204, 92); // Yellow

  // Define the color stops
  const float stop1 = 0.0f;
  const float stop2 = 0.16f;
  const float stop3 = 0.42f;
  const float stop4 = 0.6425f;
  const float stop5 = 0.8575f;
  const float stop6 = 0.93f;
  const float stop7 = 1.0f;

  Color result;

  if (t < stop2)
  {
    float alpha = (t - stop1) / (stop2 - stop1);
    result = Color(
        color1.r + (color2.r - color1.r) * alpha,
        color1.g + (color2.g - color1.g) * alpha,
        color1.b + (color2.b - color1.b) * alpha);
  }
  else if (t < stop3)
  {
    float alpha = (t - stop2) / (stop3 - stop2);
    result = Color(
        color2.r + (color3.r - color2.r) * alpha,
        color2.g + (color3.g - color2.g) * alpha,
        color2.b + (color3.b - color2.b) * alpha);
  }
  else if (t < stop4)
  {
    float alpha = (t - stop3) / (stop4 - stop3);
    result = Color(
        color3.r + (color4.r - color3.r) * alpha,
        color3.g + (color4.g - color3.g) * alpha,
        color3.b + (color4.b - color3.b) * alpha);
  }
  else if (t < stop5)
  {
    float alpha = (t - stop4) / (stop5 - stop4);
    result = Color(
        color4.r + (color5.r - color4.r) * alpha,
        color4.g + (color5.g - color4.g) * alpha,
        color4.b + (color5.b - color4.b) * alpha);
  }
  else if (t < stop6)
  {
    float alpha = (t - stop5) / (stop6 - stop5);
    result = Color(
        color5.r + (color6.r - color5.r) * alpha,
        color5.g + (color6.g - color5.g) * alpha,
        color5.b + (color6.b - color5.b) * alpha);
  }
  else
  {
    float alpha = (t - stop6) / (stop7 - stop6);
    result = Color(
        color6.r + (color7.r - color6.r) * alpha,
        color6.g + (color7.g - color6.g) * alpha,
        color6.b + (color7.b - color6.b) * alpha);
  }
  return result;
}
