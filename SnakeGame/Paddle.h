#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameObject.h"

namespace ArkanoidGame
{

class Paddle : public GameObject
{
public:
    Paddle();
    void update(float deltaTime, const sf::RenderWindow* window = nullptr) override;
    void draw(sf::RenderWindow& window) override;
    sf::Vector2f getSize() const;
    sf::FloatRect getBounds() const override;
    float getVelocityX() const { return currentVelocityX; }

private:
    sf::RectangleShape shape;
    float width;
    float height;
    float speed;
    float currentVelocityX = 0.0f;
    sf::Vector2i previousMousePos;
    
    void initializeShape();
};

} // namespace ArkanoidGame

