#include "Apple.h"
#include "Constants.h"
#include "Math.h"
#include <cassert>

using namespace SnakeGame;

Apple::Apple()
    : position(0, 0)
{
}

void Apple::InitApple()
{
    // Генерируем случайную позицию в игровой области (между стенами)
    // Стены на позициях x=1, x=GRID_WIDTH, y=1, y=GRID_HEIGHT
    // Игровая область: от 2 до GRID_WIDTH-1 (X) и от 2 до GRID_HEIGHT-1 (Y)
    sf::Vector2i randomPos = GetRandomPositionInScreen(GRID_WIDTH - 2, GRID_HEIGHT - 2);
    // Добавляем смещение +2, чтобы попасть в игровую область (пропускаем стены)
    position.x = randomPos.x + 2;
    position.y = randomPos.y + 2;

    assert(appleTexture.loadFromFile(RESOURCES_PATH + "apple.png"));

    // Настраиваем спрайт (член класса, не локальная переменная)
    sprite.setTexture(appleTexture);
    // Устанавливаем размер спрайта до CELL_SIZE x CELL_SIZE
    SetSpriteSize(sprite, static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE));
    // Преобразуем координаты сетки в пиксели и устанавливаем позицию
    sprite.setPosition(GridToPixel(position));
}

void Apple::DrawApple(sf::RenderWindow& window)
{
    window.draw(sprite);
}