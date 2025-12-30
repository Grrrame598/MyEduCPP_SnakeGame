#include "Block.h"
#include "Constants.h"

namespace SnakeGame
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
    shape.setOutlineThickness(1.0f);
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
    shape.setFillColor(sf::Color::Red);
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

StrongBlock::StrongBlock(const sf::Vector2f& position, int hitsRequired)
    : Block(position),
      hitsRequired(hitsRequired),
      currentHits(0)
{
    shape.setFillColor(sf::Color(255, 165, 0));  
}

void StrongBlock::onHit()
{
    currentHits++;
    if (currentHits == 1)
    {
        shape.setFillColor(sf::Color(255, 200, 100));
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

BonusBlock::BonusBlock(const sf::Vector2f& position)
    : Block(position),
      destroyed(false)
{
    shape.setFillColor(sf::Color::Green);
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

UnbreakableBlock::UnbreakableBlock(const sf::Vector2f& position)
    : Block(position)
{
    shape.setFillColor(sf::Color(128, 128, 128));  // Серый цвет
}

void UnbreakableBlock::onHit()
{
}

bool UnbreakableBlock::isDestroyed() const
{
    return false;
}

int UnbreakableBlock::getHitsRemaining() const
{
    return -1;
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

void TripleBlock::updateColor()
{
    // Сиреневые оттенки: от темного к светлому при каждом попадании
    // hit 0: темно-сиреневый (138, 43, 226) - Indigo
    // hit 1: средний сиреневый (186, 85, 211) - MediumOrchid
    // hit 2: светлый сиреневый (221, 160, 221) - Plum
    switch (currentHits)
    {
    case 0:
        shape.setFillColor(sf::Color(138, 43, 226));  // Темно-сиреневый
        break;
    case 1:
        shape.setFillColor(sf::Color(186, 85, 211));  // Средний сиреневый
        break;
    case 2:
        shape.setFillColor(sf::Color(221, 160, 221)); // Светлый сиреневый
        break;
    default:
        shape.setFillColor(sf::Color(221, 160, 221)); // Светлый сиреневый
        break;
    }
}

} // namespace SnakeGame

