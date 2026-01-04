#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameObject.h"

namespace ArkanoidGame
{

enum class BonusType
{
    PADDLE_SIZE,    // Увеличение платформы
    SPEED,          // Ускорение шарика
    ARMOR           // Бронебойный шарик
};

class FallingBonus : public GameObject
{
public:
    FallingBonus(const sf::Vector2f& position, BonusType type);
    
    void update(float deltaTime, const sf::RenderWindow* window = nullptr) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    
    BonusType getType() const { return type; }
    sf::Vector2f getSize() const { return sf::Vector2f(width, height); }

private:
    sf::RectangleShape shape;
    BonusType type;
    float width;
    float height;
    float fallSpeed;  // Скорость падения вниз
    
    void initializeShape();
    sf::Color getColorForType(BonusType bonusType) const;
};

} // namespace ArkanoidGame

