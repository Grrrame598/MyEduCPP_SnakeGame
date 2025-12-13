#include "Math.h"
#include "Constants.h"

namespace SnakeGame
{

void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight)
{
    sf::FloatRect spriteRect = sprite.getLocalBounds();
    sf::Vector2f scale = { desiredWidth / spriteRect.width, desiredHeight / spriteRect.height };
    sprite.setScale(scale);
}

sf::Vector2i GetRandomPositionInScreen(int screenWidth, int screenHeight)
{
    sf::Vector2i result;
    result.x = static_cast<int>(rand() / (float)RAND_MAX * screenWidth);
    result.y = static_cast<int>(rand() / (float)RAND_MAX * screenHeight);
    return result;
}

sf::Vector2f GridToPixel(const sf::Vector2i& gridPosition)
{
    // Преобразование координат сетки в пиксели
    // Координаты сетки начинаются с 1, поэтому вычитаем 1
    // Например: gridPosition(1,1) → pixel(0,0), gridPosition(2,1) → pixel(20,0)
    float pixelX = static_cast<float>((gridPosition.x - 1) * CELL_SIZE);
    float pixelY = static_cast<float>((gridPosition.y - 1) * CELL_SIZE);
    return sf::Vector2f(pixelX, pixelY);
}

} // namespace SnakeGame

