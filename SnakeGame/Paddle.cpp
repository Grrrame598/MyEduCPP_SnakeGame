#include "Paddle.h"
#include "Constants.h"

namespace SnakeGame
{

Paddle::Paddle()
    : width(3 * CELL_SIZE),
      height(1 * CELL_SIZE),
      speed(200.0f),
      position((SCREEN_WIDTH - 3 * CELL_SIZE) / 2.0f, SCREEN_HEIGHT - 1 * CELL_SIZE - 10.0f)
{
    initializeShape();
}

void Paddle::initializeShape()
{
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
}

void Paddle::update(float deltaTime, const sf::RenderWindow& window)
{
    sf::Vector2i mousePosInt = sf::Mouse::getPosition(window);
    float mouseX = static_cast<float>(mousePosInt.x);
    
    if (mouseX >= 0.0f && mouseX <= static_cast<float>(SCREEN_WIDTH))
    {
        position.x = mouseX - width / 2.0f;
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            position.x -= speed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            position.x += speed * deltaTime;
    }
    
    if (position.x < 0.0f)
        position.x = 0.0f;
    if (position.x + width > SCREEN_WIDTH)
        position.x = SCREEN_WIDTH - width;
    
    shape.setPosition(position);
}

} // namespace SnakeGame

