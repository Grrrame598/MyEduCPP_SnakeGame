#pragma once
#include <SFML/Graphics.hpp>

namespace ArkanoidGame
{

class Application
{
public:
    Application() = default;
    virtual ~Application() = default;
    
    virtual void update(float deltaTime, const sf::RenderWindow* window = nullptr) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void handleInput(sf::Keyboard::Key key) = 0;
};

} // namespace ArkanoidGame


