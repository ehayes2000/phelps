#include <iostream>
#include "sfmlFluid.hpp"
#define SCALE 4

sf::Color hsv_to_rgb(float h, float s, float v) {
    h = fmod(h, 360.0f);
    s = std::max(0.0f, std::min(1.0f, s));
    v = std::max(0.0f, std::min(1.0f, v));

    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    float r, g, b;
    if (h >= 0 && h < 60) {
        r = c; g = x; b = 0;
    } else if (h >= 60 && h < 120) {
        r = x; g = c; b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0; g = c; b = x;
    } else if (h >= 180 && h < 240) {
        r = 0; g = x; b = c;
    } else if (h >= 240 && h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }

    return sf::Color(
        static_cast<sf::Uint8>((r + m) * 255),
        static_cast<sf::Uint8>((g + m) * 255),
        static_cast<sf::Uint8>((b + m) * 255)
    );
}

void SfmlFluid::startRenderLoop()
{
    win.setFramerateLimit(60);
    sf::Image image;
    image.create(WIN_SIZE, WIN_SIZE, sf::Color::Black);
    sf::Texture texture;
    sf::Sprite sprite;

    while (win.isOpen())
    {
        sf::Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.close();
        }
        // Update fluid
        int cx = static_cast<int>((0.5f * WIN_SIZE) / SCALE);
        int cy = static_cast<int>((0.5f * WIN_SIZE) / SCALE);

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                fluid.addDensity(cx + i, cy + j, rand() % 101 + 50); // random(50, 150)
            }
        }

        static float t = 0;
        for (int i = 0; i < 2; i++)
        {
            float angle = std::sin(t) * 2 * M_PI * 2; // Using sin instead of noise
            float vx = std::cos(angle) * 0.2f;
            float vy = std::sin(angle) * 0.2f;
            t += 0.01f;
            fluid.addVelocity(cx, cy, vx, vy);
        }
        fluid.step();
      
        for (int i = 0; i < WIN_SIZE; i++)
        {
            for (int j = 0; j < WIN_SIZE; j++)
            {
                int x = i / SCALE;
                int y = j / SCALE;
                float d = fluid.density[x + y * WIN_SIZE];
                fluid.density[x + y * WIN_SIZE] = std::max(0.f, std::min(d - .01f, 255.f));
                float vx = fluid.Vx[x + y * WIN_SIZE];
                float vy = fluid.Vy[x + y * WIN_SIZE];
                unsigned int vmag = static_cast<unsigned int>(std::sqrt(vx*vx + vy*vy) * 100000) % 255;
 
                image.setPixel(i, j, sf::Color(vmag, 0, 255, d));
            }
        }
        texture.loadFromImage(image);
        sprite.setTexture(texture);

        win.clear();
        win.draw(sprite);
        win.display();
    }
}