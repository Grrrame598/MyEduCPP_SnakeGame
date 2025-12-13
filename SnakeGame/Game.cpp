#include "Game.h"
#include "Constants.h"
#include "SoundManager.h"

using namespace SnakeGame;

void Game::initialize(int P)
{
    pointsPerApple = P;
    gameField.initialize();
    snake.initSnake();
    
    do {
        apple.InitApple();
    } while (snake.isPositionOccupied(apple.getPosition()));
    
    numEatenApples = 0;
}

void Game::update(float deltaTime, int V, int L, SoundManager* soundManager)
{
    snake.update(deltaTime, V);
    
    if (snake.checkCollisions(apple.getPosition()))
    {
        snake.grow(L);
        ++numEatenApples;
        
        do {
            apple.InitApple();
        } while (snake.isPositionOccupied(apple.getPosition()));
        
        if (soundManager)
            soundManager->playAppleEat();
    }
}

void Game::draw(sf::RenderWindow& window)
{
    gameField.drawField(window);
    gameField.drawWalls(window);
    apple.DrawApple(window);
    snake.draw(window);
}

void Game::handleInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::W || key == sf::Keyboard::Up)
        snake.setDirection(Direction::UP);
    else if (key == sf::Keyboard::S || key == sf::Keyboard::Down)
        snake.setDirection(Direction::DOWN);
    else if (key == sf::Keyboard::A || key == sf::Keyboard::Left)
        snake.setDirection(Direction::LEFT);
    else if (key == sf::Keyboard::D || key == sf::Keyboard::Right)
        snake.setDirection(Direction::RIGHT);
}