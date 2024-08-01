#include "sfmlFluid.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    SfmlFluid fluid(720, 480);
    fluid.startRenderLoop();
    return 0;
}

// #include <SFML/Graphics.hpp>
// #include <vector>
// #include <cmath>

// struct PressurePoint {
//     sf::Vector2f position;
//     float pressure;
//     float radius;
// };

// class PressureField {
// private:
//     sf::RenderTexture renderTexture;
//     std::vector<PressurePoint> pressurePoints;
//     sf::Shader kernelShader;

// public:
//     PressureField(unsigned int width, unsigned int height) {
//         renderTexture.create(width, height);
        
//         // Load and set up the shader
//         if (!kernelShader.loadFromFile("kernel_shader.frag", sf::Shader::Fragment)) {
//             // Handle shader loading error
//         }
//     }

//     void addPressurePoint(const PressurePoint& point) {
//         pressurePoints.push_back(point);
//     }

//     void render() {
//         renderTexture.clear(sf::Color::Transparent);

//         sf::CircleShape circle;
//         circle.setPointCount(60);  // Smooth circle

//         for (const auto& point : pressurePoints) {
//             circle.setRadius(point.radius);
//             circle.setOrigin(point.radius, point.radius);
//             circle.setPosition(point.position);

//             kernelShader.setUniform("center", sf::Glsl::Vec2(point.position));
//             kernelShader.setUniform("radius", point.radius);
//             kernelShader.setUniform("pressure", point.pressure);

//             sf::RenderStates states;
//             states.shader = &kernelShader;
//             states.blendMode = sf::BlendAdd;  // Use additive blending

//             renderTexture.draw(circle, states);
//         }

//         renderTexture.display();
//     }

//     const sf::Texture& getTexture() const {
//         return renderTexture.getTexture();
//     }
// };

// int main() {
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Pressure Field");
//     PressureField pressureField(800, 600);

//     // Add some pressure points
//     pressureField.addPressurePoint({sf::Vector2f(300, 300), 1.0f, 100.0f});
//     pressureField.addPressurePoint({sf::Vector2f(400, 400), -0.7f, 150.0f});
//     pressureField.addPressurePoint({sf::Vector2f(200, 200), 0.5f, 120.0f});
//     pressureField.addPressurePoint({sf::Vector2f(600, 300), -0.3f, 80.0f});
//     // Add more points as needed

//     sf::Sprite fieldSprite;

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         pressureField.render();
//         fieldSprite.setTexture(pressureField.getTexture());

//         window.clear(sf::Color::White);
//         window.draw(fieldSprite);
//         window.display();
//     }

//     return 0;
// }