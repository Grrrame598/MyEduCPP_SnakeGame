#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

namespace SnakeGame
{

// Базовый класс для всех игровых объектов
class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject() = default;
    
    // Обновление состояния объекта (window опционален, нужен только для Paddle)
    virtual void update(float deltaTime, const sf::RenderWindow* window = nullptr) = 0;
    
    // Отрисовка объекта
    virtual void draw(sf::RenderWindow& window) = 0;
    
    // Получение позиции объекта
    sf::Vector2f getPosition() const { return position; }
    
    // Получение границ объекта для проверки коллизий
    virtual sf::FloatRect getBounds() const = 0;

protected:
    sf::Vector2f position; // Позиция объекта
};

} // namespace SnakeGame

