#pragma once
#include "Constants.h"

namespace ArkanoidGame
{

// Класс для управления состоянием игры (RAII)
class GameStateManager
{
private:
    GameState currentState;

public:
    GameStateManager() : currentState(GameState::MENU) {}
    ~GameStateManager() = default;
    
    GameState getCurrentState() const { return currentState; }
    void setState(GameState newState) { currentState = newState; }
};

extern GameStateManager gameStateManager;

} // namespace ArkanoidGame

