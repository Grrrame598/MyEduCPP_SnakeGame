#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

namespace ArkanoidGame
{

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;
    
    virtual void update(float deltaTime, const sf::RenderWindow* window = nullptr) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    sf::Vector2f getPosition() const { return position; }
    virtual sf::FloatRect getBounds() const = 0;

protected:
    sf::Vector2f position;
};

} // namespace ArkanoidGame

