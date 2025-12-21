#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

namespace SnakeGame
{

// Класс для управления шариком
class Ball
{
public:
    Ball();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const { return position; }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }

private:
    sf::CircleShape shape;
    float radius;
    float speed;
    sf::Vector2f position;
    sf::Vector2f velocity;
    
    void initializeShape();
};

} // namespace SnakeGame

