#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameObject.h"

namespace ArkanoidGame
{

class Ball : public GameObject
{
public:
    Ball();
    void update(float deltaTime, const sf::RenderWindow* window = nullptr) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    
    float getRadius() const { return radius; }
    sf::Vector2f getCenter() const;
    sf::Vector2f getVelocity() const { return velocity; }
    void setVelocityX(float vx);
    void setVelocity(const sf::Vector2f& v);
    void setPosition(const sf::Vector2f& pos);

private:
    sf::CircleShape shape;
    float radius;
    float speed;
    sf::Vector2f velocity;
    
    void initializeShape();
};

} // namespace ArkanoidGame

