#include "Block.h"
#include "Constants.h"

namespace ArkanoidGame
{

Block::Block(const sf::Vector2f& position)
    : GameObject(),
      width(2 * CELL_SIZE),
      height(CELL_SIZE)
{
    this->position = position;
    initializeShape();
}

void Block::initializeShape()
{
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(position);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineThickness(BLOCK_OUTLINE_THICKNESS);
    shape.setOutlineColor(sf::Color(0, 0, 0, 128));
}

void Block::update(float deltaTime, const sf::RenderWindow* window)
{
    shape.setPosition(position);
}

void Block::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Block::getBounds() const
{
    return shape.getGlobalBounds();
}

SimpleBlock::SimpleBlock(const sf::Vector2f& position)
    : Block(position),
      destroyed(false)
{
    // Устанавливаем цвет после инициализации формы в базовом классе
    shape.setFillColor(SIMPLE_BLOCK_COLOR);
}

void SimpleBlock::onHit()
{
    destroyed = true;
}

bool SimpleBlock::isDestroyed() const
{
    return destroyed;
}

int SimpleBlock::getHitsRemaining() const
{
    if (destroyed)
        return 0;
    else
        return 1;
}

int SimpleBlock::getPoints() const
{
    return 1;
}

StrongBlock::StrongBlock(const sf::Vector2f& position, int hitsRequired)
    : Block(position),
      hitsRequired(hitsRequired),
      currentHits(0)
{
    shape.setFillColor(STRONG_BLOCK_COLOR);  
}

void StrongBlock::onHit()
{
    currentHits++;
    if (currentHits == 1)
    {
        shape.setFillColor(STRONG_BLOCK_DAMAGED_COLOR);
    }
}

bool StrongBlock::isDestroyed() const
{
    return currentHits >= hitsRequired;
}

int StrongBlock::getHitsRemaining() const
{
    int remaining = hitsRequired - currentHits;
    if (remaining > 0)
        return remaining;
    else
        return 0;
}

int StrongBlock::getPoints() const
{
    return 2;
}

BonusBlock::BonusBlock(const sf::Vector2f& position)
    : Block(position),
      destroyed(false)
{
    shape.setFillColor(BONUS_BLOCK_COLOR);
}

void BonusBlock::onHit()
{
    destroyed = true;
}

bool BonusBlock::isDestroyed() const
{
    return destroyed;
}

int BonusBlock::getHitsRemaining() const
{
    if (destroyed)
        return 0;
    else
        return 1;
}

int BonusBlock::getPoints() const
{
    return 1;
}

UnbreakableBlock::UnbreakableBlock(const sf::Vector2f& position)
    : Block(position)
{
    shape.setFillColor(UNBREAKABLE_BLOCK_COLOR);
}

void UnbreakableBlock::onHit()
{
    // Обычный удар не уничтожает неубиваемый блок
}

bool UnbreakableBlock::isDestroyed() const
{
    return destroyedByArmor;
}

void UnbreakableBlock::destroyByArmor()
{
    destroyedByArmor = true;
}

int UnbreakableBlock::getHitsRemaining() const
{
    return -1;
}

int UnbreakableBlock::getPoints() const
{
    // Возвращаем 4 очка, если блок уничтожен бронебойным шариком
    // Проверка на бронебойность будет в Game.cpp
    return 4;
}

TripleBlock::TripleBlock(const sf::Vector2f& position)
    : Block(position),
      currentHits(0)
{
    updateColor();
}

void TripleBlock::onHit()
{
    currentHits++;
    updateColor();
}

bool TripleBlock::isDestroyed() const
{
    return currentHits >= 3;
}

int TripleBlock::getHitsRemaining() const
{
    int remaining = 3 - currentHits;
    if (remaining > 0)
        return remaining;
    else
        return 0;
}

int TripleBlock::getPoints() const
{
    return 3;
}

void TripleBlock::updateColor()
{
    // Сиреневые оттенки: от темного к светлому при каждом попадании
    // hit 0: темно-сиреневый (138, 43, 226) - Indigo
    // hit 1: средний сиреневый (186, 85, 211) - MediumOrchid
    // hit 2: светлый сиреневый (221, 160, 221) - Plum
    switch (currentHits)
    {
    case 0:
        shape.setFillColor(TRIPLE_BLOCK_COLOR_DARK);
        break;
    case 1:
        shape.setFillColor(TRIPLE_BLOCK_COLOR_MEDIUM);
        break;
    case 2:
        shape.setFillColor(TRIPLE_BLOCK_COLOR_LIGHT);
        break;
    default:
        shape.setFillColor(TRIPLE_BLOCK_COLOR_LIGHT);
        break;
    }
}

} // namespace ArkanoidGame

