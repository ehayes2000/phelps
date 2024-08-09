#include "sfmlFluid.hpp"
#include "ui.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <algorithm>

sf::CircleShape SfmlFluid::makeDrawable(const Particle &p) const
{
  sf::CircleShape c = sf::CircleShape(fluid.params.renderRadius * fluid.getScale());
  Vec rCords = fluid.stor(p.position);
  float speed = p.velocity.mag();
  sf::Color color = plasmaGradient(speed, 0, .3f); // TODO figure out a real upper bound
  c.setPosition(rCords.x, rCords.y);
  c.setFillColor(color);
  return c;
}

void SfmlFluid::drawParticles() 
{  
  const auto &particles = fluid.getParticles();
  for (const auto &p: particles){
    const auto drawable = makeDrawable(p);
    window.draw(drawable);
  }
}

sf::Color getPressureColor(float pressure) {
    // Convert pressure to a value between 0 and 1
    float t = (pressure + 1.0f) / 2.0f;
     
    int r = static_cast<int>(std::min(255.0f, 510.0f * t));
    int b = static_cast<int>(std::min(255.0f, 510.0f * (1.0f - t)));
    int g = 255 - std::abs(r - b);

    return sf::Color(r, g, b);
}

void SfmlFluid::handleSfEvent(sf::Event &event) 
{ 
  ImGui::SFML::ProcessEvent(event);
  if (event.type == sf::Event::Closed)
  {
    window.close();
  }
  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
    params.isDebugMenu = !params.isDebugMenu;
  }
  if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
  {
    params.isPaused = !params.isPaused;
  }
  if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
  {
    params.isLClick = true;
  }
  if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left){
    params.isLClick = false;
  }
  if (params.isLClick && !params.isPaused && !params.isDebugMenu){
    sf::Vector2i mPos = sf::Mouse::getPosition(window);
    Vec simPoint = fluid.rtos(Vec(mPos.x, mPos.y));
    fluid.applyForce(simPoint, .2, 0.2f);
  }
}

const sf::Sprite SfmlFluid::getDensityImage() const { 
  std::vector<std::vector<float>> densityGrid(height, std::vector<float>(width, 0.0f));
  for (auto &v: densityGrid){
    for (int i = 0; i < v.size(); i++){
      v[i] = 0.f;
    }
  }
  fluid.computeDensityGrid(densityGrid);
  std::cout << "Avg Density: " << fluid.computeAvgDensity() << std::endl;
  sf::Image image;
  image.create(width, height);
  for (int y = 0; y < height; y++){
    for (int x = 0; x < width; x++){
      sf::Color c = getPressureColor(densityGrid[y][x]);
      image.setPixel(x, y, c);
    }
  }
  sf::Texture texture;
  texture.loadFromImage(image);
  sf::Sprite sprite(texture);
  return sprite;
}

void SfmlFluid::startRenderLoop()
{
  sf::Clock clock;
  sf::Time dt = clock.restart();
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);
  ImGui::GetIO().FontGlobalScale = params.guiScale;
  fluid.step(dt.asSeconds());
  while (window.isOpen())
  {
    // Process events
    sf::Event event;
    while (window.pollEvent(event))
    {
      handleSfEvent(event);
    }
    window.clear();
    if (params.isDensityView){
      window.draw(getDensityImage());
    }
    else if (params.isAdjacentView){
      drawParticles();
      highlightAdjacentParticles();
    }
    else { 
      drawParticles();
    }
    if (params.isDebugMenu) { 
      ImGui::SFML::Update(window, dt);
      ShowUi(params, fluidParams);
      ImGui::SFML::Render(window);
    }
    else if (!params.isPaused){
      fluid.step(dt.asSeconds());
    }
    dt = clock.restart();
    window.display();
  }
  ImGui::SFML::Shutdown();
}

sf::Color SfmlFluid::plasmaGradient(float value, float minValue, float maxValue) const {
    // Normalize the value to a range between 0 and 1
    float t = (value - minValue) / (maxValue - minValue);
    t = std::clamp(t, 0.0f, 1.0f);

    // Define key colors for the plasma gradient
    const sf::Color color1(13, 8, 135);    // Dark blue
    const sf::Color color2(84, 2, 163);    // Purple
    const sf::Color color3(139, 10, 165);  // Magenta
    const sf::Color color4(185, 50, 137);  // Pink
    const sf::Color color5(219, 92, 104);  // Light red
    const sf::Color color6(244, 143, 77);  // Orange
    const sf::Color color7(254, 204, 92);  // Yellow

    // Define the color stops
    const float stop1 = 0.0f;
    const float stop2 = 0.16f;
    const float stop3 = 0.42f;
    const float stop4 = 0.6425f;
    const float stop5 = 0.8575f;
    const float stop6 = 0.93f;
    const float stop7 = 1.0f;

    sf::Color result;

    if (t < stop2) {
        float alpha = (t - stop1) / (stop2 - stop1);
        result = sf::Color(
            color1.r + (color2.r - color1.r) * alpha,
            color1.g + (color2.g - color1.g) * alpha,
            color1.b + (color2.b - color1.b) * alpha
        );
    } else if (t < stop3) {
        float alpha = (t - stop2) / (stop3 - stop2);
        result = sf::Color(
            color2.r + (color3.r - color2.r) * alpha,
            color2.g + (color3.g - color2.g) * alpha,
            color2.b + (color3.b - color2.b) * alpha
        );
    } else if (t < stop4) {
        float alpha = (t - stop3) / (stop4 - stop3);
        result = sf::Color(
            color3.r + (color4.r - color3.r) * alpha,
            color3.g + (color4.g - color3.g) * alpha,
            color3.b + (color4.b - color3.b) * alpha
        );
    } else if (t < stop5) {
        float alpha = (t - stop4) / (stop5 - stop4);
        result = sf::Color(
            color4.r + (color5.r - color4.r) * alpha,
            color4.g + (color5.g - color4.g) * alpha,
            color4.b + (color5.b - color4.b) * alpha
        );
    } else if (t < stop6) {
        float alpha = (t - stop5) / (stop6 - stop5);
        result = sf::Color(
            color5.r + (color6.r - color5.r) * alpha,
            color5.g + (color6.g - color5.g) * alpha,
            color5.b + (color6.b - color5.b) * alpha
        );
    } else {
        float alpha = (t - stop6) / (stop7 - stop6);
        result = sf::Color(
            color6.r + (color7.r - color6.r) * alpha,
            color6.g + (color7.g - color6.g) * alpha,
            color6.b + (color7.b - color6.b) * alpha
        );
    }
    return result;
}

void SfmlFluid::highlightAdjacentParticles(){   
  const GridView& grid = fluid.getGrid();
  sf::Vector2i mPos = sf::Mouse::getPosition(window);
  Vec simPos = fluid.rtos(Vec(mPos.x, mPos.y));
  for (const auto &p: grid.adj(simPos)) { 
    sf::CircleShape c = sf::CircleShape(fluid.params.renderRadius * fluid.getScale());
    Vec rCords = fluid.stor(p.position);
    c.setPosition(rCords.x, rCords.y);
    c.setFillColor(sf::Color::Green);
    window.draw(c);
  }
} 