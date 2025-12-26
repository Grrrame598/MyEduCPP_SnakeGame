#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameObject.h"

namespace SnakeGame
{

// Класс для управления шариком
class Ball : public GameObject
{
public:
    Ball();
    void update(float deltaTime, const sf::RenderWindow* window = nullptr) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

private:
    sf::CircleShape shape;
    float radius;
    float speed;
    sf::Vector2f velocity;
    
    void initializeShape();
};

} // namespace SnakeGame

