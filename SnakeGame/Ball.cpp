#include "Ball.h"
#include "Constants.h"

namespace ArkanoidGame
{

Ball::Ball()
    : GameObject(),
      radius(BALL_RADIUS),
      speed(BALL_INITIAL_SPEED),
      velocity(speed * BALL_INITIAL_VELOCITY_X_COEFF, -speed * BALL_INITIAL_VELOCITY_Y_COEFF)
{
    position = sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 2 * CELL_SIZE - 10.0f - BALL_START_OFFSET_Y);
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

void Ball::setVelocityX(float vx)
{
    velocity.x = vx;
}

void Ball::setVelocity(const sf::Vector2f& v)
{
    velocity = v;
}

void Ball::setPosition(const sf::Vector2f& pos)
{
    position = pos;
    shape.setPosition(position);
}

} // namespace ArkanoidGame

