#include "Ball.h"
#include "Constants.h"

namespace SnakeGame
{

Ball::Ball()
    : radius(CELL_SIZE),
      speed(300.0f),
      position(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 2 * CELL_SIZE - 10.0f),
      velocity(speed * 0.5f, -speed * 0.8660254f)
{
    initializeShape();
}

void Ball::initializeShape()
{
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
}

void Ball::update(float deltaTime)
{
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    shape.setPosition(position);
}

} // namespace SnakeGame

