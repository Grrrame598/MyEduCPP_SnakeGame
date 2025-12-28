#include "Paddle.h"
#include "Constants.h"

namespace SnakeGame
{

Paddle::Paddle()
    : GameObject(),
      width(3 * CELL_SIZE),
      height(1 * CELL_SIZE),
      speed(600.0f),
      currentVelocityX(0.0f),
      previousMousePos(-1, -1)
{
    position = sf::Vector2f((SCREEN_WIDTH - 3 * CELL_SIZE) / 2.0f, SCREEN_HEIGHT - 1 * CELL_SIZE - 10.0f - 80.0f);
    initializeShape();
}

void Paddle::initializeShape()
{
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
}

void Paddle::update(float deltaTime, const sf::RenderWindow* window)
{
    float previousX = position.x;
    currentVelocityX = 0.0f;
    
    bool keyboardUsed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    
    if (keyboardUsed)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            currentVelocityX = -speed;
            position.x += currentVelocityX * deltaTime;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            currentVelocityX = speed;
            position.x += currentVelocityX * deltaTime;
        }
        previousMousePos = sf::Vector2i(-1, -1);
    }
    else if (window != nullptr)
    {
        sf::Vector2i currentMousePos = sf::Mouse::getPosition(*window);
        
        if (previousMousePos.x != -1 && 
            (currentMousePos.x != previousMousePos.x || currentMousePos.y != previousMousePos.y))
        {
            float mouseX = static_cast<float>(currentMousePos.x);
            position.x = mouseX - width / 2.0f;
        }
        
        previousMousePos = currentMousePos;
    }
    
    if (deltaTime > 0.0f)
    {
        currentVelocityX = (position.x - previousX) / deltaTime;
    }
    
    if (position.x < 0.0f)
        position.x = 0.0f;
    if (position.x + width > SCREEN_WIDTH)
        position.x = SCREEN_WIDTH - width;
    
    shape.setPosition(position);
}

void Paddle::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::Vector2f Paddle::getSize() const
{
    return sf::Vector2f(width, height);
}

sf::FloatRect Paddle::getBounds() const
{
    return shape.getGlobalBounds();
}

} // namespace SnakeGame

