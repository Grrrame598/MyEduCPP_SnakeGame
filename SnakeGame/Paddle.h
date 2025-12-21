#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

namespace SnakeGame
{

// Класс для управления платформой
class Paddle
{
public:
    Paddle();
    void update(float deltaTime, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
    float width;
    float height;
    float speed;
    sf::Vector2f position;
    
    void initializeShape();
};

} // namespace SnakeGame

