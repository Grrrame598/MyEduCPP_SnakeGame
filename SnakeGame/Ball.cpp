#include "Ball.h"
#include "Constants.h"

namespace SnakeGame
{

Ball::Ball()
    : GameObject(),
      radius(CELL_SIZE * 0.5f),
      speed(450.0f),
      velocity(speed * 0.5f, -speed * 0.8660254f)
{
    position = sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 2 * CELL_SIZE - 10.0f - 80.0f);
    initializeShape();
}

void Ball::initializeShape()
{
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
}

void Ball::update(float deltaTime, const sf::RenderWindow* window)
{
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    shape.setPosition(position);
}

void Ball::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Ball::getBounds() const
{
    return shape.getGlobalBounds();
}

sf::Vector2f Ball::getCenter() const
{
    return position;
}

void Ball::reflectX()
{
    velocity.x = -velocity.x;
}

void Ball::reflectY()
{
    velocity.y = -velocity.y;
}

void Ball::setVelocityX(float vx)
{
    velocity.x = vx;
}

void Ball::setVelocity(const sf::Vector2f& v)
{
    velocity = v;
}

} // namespace SnakeGame

