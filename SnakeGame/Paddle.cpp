#include "Paddle.h"
#include "Constants.h"

namespace SnakeGame
{

Paddle::Paddle()
    : GameObject(),
      width(3 * CELL_SIZE),
      height(1 * CELL_SIZE),
      speed(200.0f)
{
    position = sf::Vector2f((SCREEN_WIDTH - 3 * CELL_SIZE) / 2.0f, SCREEN_HEIGHT - 1 * CELL_SIZE - 10.0f);
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
    bool keyboardUsed = false;
    
    // Проверяем клавиатуру (имеет приоритет)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        position.x -= speed * deltaTime;
        keyboardUsed = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        position.x += speed * deltaTime;
        keyboardUsed = true;
    }
    
    // Если клавиатура не используется и есть окно - используем мышь
    if (!keyboardUsed && window)
    {
        sf::Vector2i mousePosInt = sf::Mouse::getPosition(*window);
        float mouseX = static_cast<float>(mousePosInt.x);
        
        if (mouseX >= 0.0f && mouseX <= static_cast<float>(SCREEN_WIDTH))
        {
            position.x = mouseX - width / 2.0f;
        }
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

