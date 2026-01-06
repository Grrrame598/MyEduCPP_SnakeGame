#include "GameObjectFactory.h"
#include "Constants.h"

namespace ArkanoidGame
{

std::unique_ptr<Block> GameObjectFactory::createBlock(BlockType type, const sf::Vector2f& position)
{
    switch (type)
    {
    case BlockType::SIMPLE:
        return std::make_unique<SimpleBlock>(position);
        
    case BlockType::STRONG:
        return std::make_unique<StrongBlock>(position, STRONG_BLOCK_HITS_REQUIRED);
        
    case BlockType::TRIPLE:
        return std::make_unique<TripleBlock>(position);
        
    case BlockType::UNBREAKABLE:
        return std::make_unique<UnbreakableBlock>(position);
        
    default:
        // По умолчанию создаем простой блок
        return std::make_unique<SimpleBlock>(position);
    }
}

std::unique_ptr<FallingBonus> GameObjectFactory::createBonus(BonusType type, const sf::Vector2f& position)
{
    return std::make_unique<FallingBonus>(position, type);
}

} // namespace ArkanoidGame

