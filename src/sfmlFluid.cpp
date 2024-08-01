#include "sfmlFluid.hpp"

sf::CircleShape *SfmlFluid::makeDrawable(const Particle &p)
{
  sf::CircleShape *c = new sf::CircleShape(fluid.radius * scale);
  Vec rCords = stor(p.pos);
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
    Vec rCords = stor(fluid.getParticles()[i].pos);
    drawables[i]->setPosition(
        rCords.x,
        rCords.y);
  }
}

void SfmlFluid::startRenderLoop()
{

  // Create a window with a resolution of 800x600 pixels and a title "SFML Circle"
  sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "Phelps");

  // Main loop that continues until the window is closed
  sf::Clock clock;
  float deltaTime = 0.0f;
  bool isPaused = false;
  bool isLClick = false;
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
        isPaused = !isPaused;
        deltaTime = clock.restart().asSeconds();
      }
      if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
      {
        isLClick = true;
        Vec simPoint = rtos(Vec(event.mouseButton.x, event.mouseButton.y));
        fluid.applyForce(simPoint, 0.002f, 0.2f);
      }
      else if (event.type == sf::Event::MouseButtonReleased)
      {
        isLClick = false;
      }
      else if (sf::Event::MouseMoved && isLClick)
      {
        sf::Vector2i mPos = sf::Mouse::getPosition(window);
        Vec simPoint = rtos(Vec(mPos.x, mPos.y));
        fluid.applyForce(simPoint, 0.01f, 0.2f);
      }
    }
    if (isPaused)
      continue;
    // Clear the window with black color
    deltaTime = clock.restart().asSeconds();
    fluid.step(deltaTime);
    updateDrawables();
    window.clear(sf::Color::White);

    // Draw the circle
    for (const auto &d : drawables)
    {
      window.draw(*d);
    }

    // Display the contents of the window
    window.display();
  }
}

Vec SfmlFluid::stor(const Vec &p) const
{
  /*
    Transorm simulation points to render points.
    Flip y axis
    offset by radius
    // p.x * scale,
    // winHeight - (p.y * scale)
  */
  return Vec(
      scale * (p.x - fluid.radius),
      scale * (fluid.boundSize.y - p.y - fluid.radius));
}

Vec SfmlFluid::rtos(const Vec &p) const
{
  // transform render point to simulation point
  // DOES NOT transform radius
  return Vec(
      p.x / scale,
      (fluid.boundSize.y - p.y / scale));
}