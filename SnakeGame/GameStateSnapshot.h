#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Block.h"

namespace ArkanoidGame
{

struct BlockState
{
    BlockType type;
    sf::Vector2f position;
    int hitsRemaining;
};

class GameStateSnapshot
{
public:
    GameStateSnapshot() = default;
    
    void setBlocks(const std::vector<BlockState>& blocks) { this->blocks = blocks; }
    const std::vector<BlockState>& getBlocks() const { return blocks; }
    
    void setScore(int score) { this->score = score; }
    int getScore() const { return score; }
    
private:
    std::vector<BlockState> blocks;
    int score = 0;
};

} // namespace ArkanoidGame

