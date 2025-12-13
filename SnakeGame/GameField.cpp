#include "GameField.h"
#include "Math.h"
#include <cassert>

using namespace SnakeGame;

GameField::GameField()
{
    width = GRID_WIDTH;
    height = GRID_HEIGHT;
}

void GameField::initialize()
{
    // Инициализация размеров поля из констант
    width = GRID_WIDTH;
    height = GRID_HEIGHT;
    
    // Инициализация цветов заливки поля (грязно-зеленые, приглушенные оттенки)
    lightGreen = sf::Color(115, 170, 115);  // Светло-грязно-зеленый
    darkGreen = sf::Color(110, 165, 110);   // Темно-грязно-зеленый (еще более осветлен, ближе к светло-зеленому)
    
    initializeWallSprites();
    initializeFieldTiles();
}

int GameField::getWidth() const
{
    return width;
}

int GameField::getHeight() const
{
    return height;
}

sf::Vector2f GameField::gridToPixel(const sf::Vector2i& gridPosition) const
{
    // Используем утилиту из Math.h для преобразования координат
    return GridToPixel(gridPosition);
}

void GameField::drawWalls(sf::RenderWindow& window) const
{
    // Отрисовка всех спрайтов стен
    for (const auto& sprite : wallSprites)
    {
        window.draw(sprite);
    }
}

void GameField::drawField(sf::RenderWindow& window) const
{
    // Отрисовка заливки поля (шахматный порядок)
    for (const auto& tile : fieldTiles)
    {
        window.draw(tile);
    }
}

void GameField::initializeFieldTiles()
{
    fieldTiles.clear();
    
    // Создаем прямоугольники для игровой области (между стенами)
    // Стены на: x=1, x=GRID_WIDTH, y=1, y=GRID_HEIGHT
    // Игровая область: x от 2 до GRID_WIDTH-1, y от 2 до GRID_HEIGHT-1
    for (int y = 2; y <= GRID_HEIGHT - 1; ++y)
    {
        for (int x = 2; x <= GRID_WIDTH - 1; ++x)
        {
            // Создаем прямоугольник размером CELL_SIZE x CELL_SIZE
            sf::RectangleShape tile(sf::Vector2f(static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE)));
            
            // Определяем цвет в зависимости от позиции (шахматный порядок)
            if ((x + y) % 2 == 0)
            {
                tile.setFillColor(lightGreen);  // Светло-зеленый
            }
            else
            {
                tile.setFillColor(darkGreen);   // Темно-зеленый
            }
            
            // Устанавливаем позицию (преобразуем координаты сетки в пиксели)
            tile.setPosition(gridToPixel(sf::Vector2i(x, y)));
            
            fieldTiles.push_back(tile);
        }
    }
}


void GameField::initializeWallSprites()
{

    for (int i = 0; i < GRID_WIDTH; ++i)
    {
    bricksPositions.insert(sf::Vector2i(i+1, 1));
    }
    for (int i = 0; i < GRID_WIDTH; ++i)
    {
    bricksPositions.insert(sf::Vector2i(i+1, GRID_HEIGHT));
    }
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
    bricksPositions.insert(sf::Vector2i(1, i+1));
    }
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
    bricksPositions.insert(sf::Vector2i(GRID_WIDTH, i+1));
    }


    wallSprites.clear();
    assert(wallTexture.loadFromFile(RESOURCES_PATH + "brick.png"));

    for (const auto& pos : bricksPositions)
    {
        sf::Sprite sprite;
        sprite.setTexture(wallTexture);
        // Устанавливаем размер спрайта до CELL_SIZE x CELL_SIZE
        SetSpriteSize(sprite, static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE));
        // Преобразуем pos (координаты сетки) в пиксели и устанавливаем позицию
        sprite.setPosition(gridToPixel(pos));
        wallSprites.push_back(sprite);
    }

   


}