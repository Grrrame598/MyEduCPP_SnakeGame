#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include "Constants.h"

namespace SnakeGame
{
// Компаратор для sf::Vector2i (нужен для std::set)
struct Vector2iComparator
{
    bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const
    {
        // Сравниваем сначала по y, потом по x
        if (a.y != b.y)
            return a.y < b.y;
        return a.x < b.x;
    }
};

// Класс для управления игровым полем (границы, проверка столкновений со стенами)
// Использует логическую сетку - координаты объектов хранятся в единицах сетки (клетки),
// а GameField проверяет, находятся ли они в пределах поля
class GameField
{
public:
    GameField();
    
    // Инициализация игрового поля (можно использовать для настройки параметров в будущем)
    void initialize();
    
    // Проверка, находится ли позиция в пределах поля
    // Возвращает true, если позиция валидна (внутри границ)
    bool isPositionValid(const sf::Vector2i& gridPosition) const;
    
    // Проверка столкновения со стеной (позиция вышла за границы поля)
    // Возвращает true, если позиция вне поля (столкновение)
    bool isOutOfBounds(const sf::Vector2i& gridPosition) const;
    
    // Получить ширину поля в клетках
    int getWidth() const;
    
    // Получить высоту поля в клетках
    int getHeight() const;
    
    // Преобразование координат сетки в пиксели (для отрисовки)
    sf::Vector2f gridToPixel(const sf::Vector2i& gridPosition) const;
    
    // Преобразование пиксельных координат в координаты сетки
    sf::Vector2i pixelToGrid(const sf::Vector2f& pixelPosition) const;
    
    // Отрисовка стен поля
    void drawWalls(sf::RenderWindow& window) const;
    
    // Отрисовка заливки поля (шахматный порядок)
    void drawField(sf::RenderWindow& window) const;
    
private:
    // Вспомогательный метод для инициализации спрайтов стен (вызывается из initialize())
    void initializeWallSprites();
    
    // Вспомогательный метод для инициализации заливки поля (вызывается из initialize())
    void initializeFieldTiles();
    
    // Размеры поля в клетках (берутся из констант GRID_WIDTH и GRID_HEIGHT)
    int width;
    int height;
    
    // Спрайты для отрисовки стен (кирпичи)
    // Создаются один раз при инициализации, затем используются для отрисовки
    std::vector<sf::Sprite> wallSprites;
    
    // Прямоугольники для заливки поля (шахматный порядок)
    // Создаются один раз при инициализации, затем используются для отрисовки
    std::vector<sf::RectangleShape> fieldTiles;
    
    // Уникальные позиции кирпичей на сетке (координаты)
    // Используем кастомный компаратор, так как sf::Vector2i не имеет operator<
    std::set<sf::Vector2i, Vector2iComparator> bricksPositions;
    
    // Текстура для стен (кирпичей)
    // TODO: будет загружаться из файла, пока можно использовать пустую текстуру для тестирования
    sf::Texture wallTexture;
    
    // Цвета для заливки поля (шахматный порядок)
    sf::Color lightGreen;
    sf::Color darkGreen;
};

} // namespace SnakeGame

