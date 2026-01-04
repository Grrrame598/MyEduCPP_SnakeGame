#include "FallingBonus.h"
#include "Constants.h"

namespace ArkanoidGame
{

FallingBonus::FallingBonus(const sf::Vector2f& position, BonusType type)
    : GameObject(),
      type(type),
      width(CELL_SIZE),
      height(CELL_SIZE),
      fallSpeed(150.0f)
{
    this->position = position;
    initializeShape();
}

void FallingBonus::initializeShape()
{
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(position);
    shape.setFillColor(getColorForType(type));
    shape.setOutlineThickness(1.0f);
    shape.setOutlineColor(sf::Color(0, 0, 0, 128));
}

void FallingBonus::update(float deltaTime, const sf::RenderWindow* window)
{
    position.y += fallSpeed * deltaTime;
    shape.setPosition(position);
}

void FallingBonus::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect FallingBonus::getBounds() const
{
    return shape.getGlobalBounds();
}

sf::Color FallingBonus::getColorForType(BonusType bonusType) const
{
    switch (bonusType)
    {
    case BonusType::PADDLE_SIZE:
        return sf::Color::Blue;  // Синий для увеличения платформы
    case BonusType::SPEED:
        return sf::Color::Green;  // Зеленый для ускорения шарика
    case BonusType::ARMOR:
        return sf::Color(255, 215, 0);  // Золотой для бронебойного шарика
    default:
        return sf::Color::White;
    }
}

} // namespace ArkanoidGame

