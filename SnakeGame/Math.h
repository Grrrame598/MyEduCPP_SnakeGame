#pragma once
#include <SFML/Graphics.hpp>
namespace SnakeGame
{
// Утилиты для математических расчетов (преобразование координат, работа с сеткой и т.д.)

// Установка размера спрайта до нужных размеров (масштабирование)
void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight);

// Получение случайной позиции в экране
sf::Vector2i GetRandomPositionInScreen(int screenWidth, int screenHeight);

// Преобразование координат сетки в пиксели (для отрисовки)
// Координаты сетки начинаются с 1, поэтому вычитаем 1
// Например: gridPosition(1,1) → pixel(0,0), gridPosition(2,1) → pixel(20,0)
sf::Vector2f GridToPixel(const sf::Vector2i& gridPosition);
}