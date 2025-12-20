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
    void initialize(int P, int V, int L, SoundManager* soundManager);
    void update(float deltaTime);
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
    int gameSpeed = 0;
    int growthLength = 0;
    SoundManager* soundManager = nullptr;
};

} // namespace SnakeGame