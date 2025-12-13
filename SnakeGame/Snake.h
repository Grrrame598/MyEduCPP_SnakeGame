#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include "Constants.h"

namespace SnakeGame
{

// Структура для представления отдельного сегмента змейки
struct Segment
{
    sf::Vector2i position;  // Позиция сегмента в сетке
    Direction direction;    // Направление движения сегмента (для отрисовки текстур)
};

// Класс для управления змейкой
class Snake
{
public:
    Snake();
    void initSnake();
    void move();
    bool checkCollisions(const sf::Vector2i& applePosition);  // Возвращает true, если яблоко съедено
    bool isPositionOccupied(const sf::Vector2i& pos) const;  // Проверяет, занята ли позиция сегментами змейки
    void grow(int L);  // L - количество сегментов для роста
    void update(float deltaTime, int V);
    void setDirection(Direction newDirection);
    void draw(sf::RenderWindow& window);  // Отрисовка змейки

private:
    std::deque<Segment> body;  // Коллекция сегментов змейки
    Direction currentDirection; // Текущее направление движения головы
    float timeSinceLastMove = 0.0f;
    
    // Текстуры для отрисовки змейки
    sf::Texture headTextures[4];      // [UP, DOWN, LEFT, RIGHT] - текстуры головы
    sf::Texture tailTextures[4];      // [UP, DOWN, LEFT, RIGHT] - текстуры хвоста
    sf::Texture bodyStraightTextures[2]; // [вертикальный, горизонтальный] - прямые сегменты тела
    sf::Texture bodyCornerTextures[4];   // [4 типа углов] - угловые сегменты тела
    

    // Загрузка текстур для отрисовки змейки
    void loadTextures();
};

} // namespace SnakeGame

