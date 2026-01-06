#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Block.h"
#include "FallingBonus.h"

namespace ArkanoidGame
{

class GameObjectFactory
{
public:
    static std::unique_ptr<Block> createBlock(BlockType type, const sf::Vector2f& position);
    static std::unique_ptr<FallingBonus> createBonus(BonusType type, const sf::Vector2f& position);
    
private:
    GameObjectFactory() = default;
};

} // namespace ArkanoidGame

