#include "sfmlFluid.hpp"
#include <algorithm>

sf::CircleShape *SfmlFluid::makeDrawable(const Particle &p)
{
  sf::CircleShape *c = new sf::CircleShape(fluid.radius * fluid.getScale());
  Vec rCords = fluid.stor(p.position);
  c->setPosition(rCords.x, rCords.y);
  c->setFillColor(sf::Color::Blue);
  return c;
}

std::vector<sf::CircleShape *> &SfmlFluid::makeDrawables()
{
  this->drawables.clear();
  this->drawables.reserve(this->fluid.getParticles().size());
  for (const auto &i : fluid.getParticles())
  {
    drawables.push_back(makeDrawable(i));
  }
  return drawables;
}

void SfmlFluid::updateDrawables()
{
  for (int i = 0; i < fluid.getParticles().size(); i++)
  {
    Vec rCords = fluid.stor(fluid.getParticles()[i].position);
    float speed = fluid.getParticles()[i].velocity.mag();
    sf::Color c = plasmaGradient(speed, 0, .3f);
    drawables[i]->setFillColor(c);
    drawables[i]->setPosition(
        rCords.x,
        rCords.y);
  }
}
void SfmlFluid::renderParticles() { 
 window.clear(sf::Color::White);

    // Draw the circle
    for (const auto &d : drawables)
    {
      window.draw(*d);
    }

    // Display the contents of the window
    window.display();
}

sf::Color getPressureColor(float pressure) {
    // Convert pressure to a value between 0 and 1
    float t = (pressure + 1.0f) / 2.0f;
     
    // Calculate RGB values
    int r = static_cast<int>(std::min(255.0f, 510.0f * t));
    int b = static_cast<int>(std::min(255.0f, 510.0f * (1.0f - t)));
    int g = 255 - std::abs(r - b);

    return sf::Color(r, g, b);
}

void SfmlFluid::startRenderLoop()
{

  // Create a window with a resolution of 800x600 pixels and a title "SFML Circle"

  // Main loop that continues until the window is closed
  sf::Clock clock;
  float deltaTime = 0.0f;
  bool isPaused = true;
  bool isLClick = false;
  std::vector<std::vector<float>> densityGrid(height, std::vector<float>(width, 0.0f));
  while (window.isOpen())
  {
    // Process events
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
      {
        for (auto &v: densityGrid){
          for (int i = 0; i < v.size(); i++){
            v[i] = 0.f;
          }
        }
        fluid.computeDensityGrid(densityGrid);
        isPaused = !isPaused;
        deltaTime = clock.restart().asSeconds();
        std::cout << "avg density: " << fluid.computeAvgDensity() << std::endl;

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
        window.draw(sprite);
        window.display();

      }
      if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
      {
        isLClick = true;
        Vec simPoint = fluid.rtos(Vec(event.mouseButton.x, event.mouseButton.y));
        fluid.applyForce(simPoint, .2, 0.2f);
      }
      else if (event.type == sf::Event::MouseButtonReleased)
      {
        isLClick = false;
      }
      else if (sf::Event::MouseMoved && isLClick)
      {
        sf::Vector2i mPos = sf::Mouse::getPosition(window);
        Vec simPoint = fluid.rtos(Vec(mPos.x, mPos.y));
        fluid.applyForce(simPoint, .2f, 0.2f);
      }
    }
    if (isPaused)
      continue;
    // Clear the window with black color
    deltaTime = clock.restart().asSeconds();
    fluid.step(deltaTime);
    updateDrawables();
    renderParticles();
   
  }
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