#pragma once
#include <SFML/Graphics.hpp>

namespace SnakeGame
{

// Класс для управления яблоком
class Apple
{
public:
    Apple();
    
    // Инициализация яблока
    void InitApple();
    
    // Отрисовка яблока
    void DrawApple(sf::RenderWindow& window);
    
    // Получение позиции яблока
    sf::Vector2i getPosition() const { return position; }
    
private:
    sf::Vector2i position;
    sf::Texture appleTexture;
    sf::Sprite sprite;
};

} // namespace SnakeGame
