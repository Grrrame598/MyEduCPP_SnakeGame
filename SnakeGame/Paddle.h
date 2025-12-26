#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameObject.h"

namespace SnakeGame
{

// Класс для управления платформой
class Paddle : public GameObject
{
public:
    Paddle();
    void update(float deltaTime, const sf::RenderWindow* window = nullptr) override;
    void draw(sf::RenderWindow& window) override;
    sf::Vector2f getSize() const;
    sf::FloatRect getBounds() const override;

private:
    sf::RectangleShape shape;
    float width;
    float height;
    float speed;
    
    void initializeShape();
};

} // namespace SnakeGame

