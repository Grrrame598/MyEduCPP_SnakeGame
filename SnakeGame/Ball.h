#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameObject.h"

namespace SnakeGame
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
    void reflectX();  // Отражение по оси X (горизонтальное)
    void reflectY();  // Отражение по оси Y (вертикальное)
    sf::Vector2f getVelocity() const { return velocity; }
    void setVelocityX(float vx);
    void setVelocity(const sf::Vector2f& v);

private:
    sf::CircleShape shape;
    float radius;
    float speed;
    sf::Vector2f velocity;
    
    void initializeShape();
};

} // namespace SnakeGame

