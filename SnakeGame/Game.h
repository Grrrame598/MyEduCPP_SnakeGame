#pragma once
#include <SFML/Graphics.hpp>
#include "GameField.h"
#include "Apple.h"
#include "Snake.h"

namespace SnakeGame
{

class SoundManager;

class Game
{
public:
    void initialize(int P);
    void update(float deltaTime, int V, int L, SoundManager* soundManager);
    void draw(sf::RenderWindow& window);
    void handleInput(sf::Keyboard::Key key);
    int getNumEatenApples() const { return numEatenApples; }
    int getScore() const { return numEatenApples * pointsPerApple; }
    
private:
    GameField gameField;
    Apple apple;
    Snake snake;
    int numEatenApples = 0;
    int pointsPerApple = 2;
};

} // namespace SnakeGame