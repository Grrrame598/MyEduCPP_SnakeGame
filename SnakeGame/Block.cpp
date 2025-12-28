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
    shape.setFillColor(sf::Color::Cyan);
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

} // namespace SnakeGame

