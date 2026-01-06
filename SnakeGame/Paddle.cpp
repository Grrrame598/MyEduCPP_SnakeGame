#include "Paddle.h"
#include "Constants.h"

namespace ArkanoidGame
{

Paddle::Paddle()
    : GameObject(),
      width(PADDLE_WIDTH_MULTIPLIER * CELL_SIZE),
      height(PADDLE_HEIGHT),
      speed(PADDLE_SPEED),
      previousMousePos(-1, -1)
{
    position = sf::Vector2f((SCREEN_WIDTH - PADDLE_WIDTH_MULTIPLIER * CELL_SIZE) / 2.0f, SCREEN_HEIGHT - PADDLE_HEIGHT - 10.0f - PADDLE_START_OFFSET_Y);
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
    bool keyboardUsed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    
    float oldX = position.x;
    
    if (keyboardUsed)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            position.x -= speed * deltaTime;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            position.x += speed * deltaTime;
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
    
    // Обновляем скорость для передачи импульса шарику
    if (deltaTime > 0.0f)
        currentVelocityX = (position.x - oldX) / deltaTime;
    else
        currentVelocityX = 0.0f;
    
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

void Paddle::setWidth(float newWidth)
{
    width = newWidth;
    shape.setSize(sf::Vector2f(width, height));
}

} // namespace ArkanoidGame

