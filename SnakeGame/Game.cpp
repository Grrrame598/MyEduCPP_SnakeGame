#include "Game.h"
#include "Constants.h"
#include "SoundManager.h"
#include <cmath> // For std::abs, std::sqrt

using namespace SnakeGame;

void Game::initialize(int P, int V, int L, SoundManager* soundManager)
{
    pointsPerApple = P;
    gameSpeed = V;
    growthLength = L;
    this->soundManager = soundManager;
    score = 0;
    initializeBlocks();
}

void Game::update(float deltaTime, const sf::RenderWindow* window)
{
    ball.update(deltaTime);
    paddle.update(deltaTime, window);
    checkBallScreenCollisions();
    checkRelativeMotionAndCollision(deltaTime);
    checkBallBlocksCollisions(deltaTime);
    
    // Восстановление скорости после бокового столкновения (через задержку)
    if (hasSideCollision)
    {
        sf::FloatRect ballBounds = ball.getBounds();
        sf::FloatRect paddleBounds = paddle.getBounds();
        
        if (!ballBounds.intersects(paddleBounds))
        {
            sideCollisionDelayFrames++;
            if (sideCollisionDelayFrames >= SPEED_RESTORE_DELAY_FRAMES)
            {
                restoreBallSpeedToInitial();
                hasSideCollision = false;
                sideCollisionDelayFrames = 0;
            }
        }
        else
        {
            sideCollisionDelayFrames = 0;
        }
    }
}

void Game::draw(sf::RenderWindow& window)
{
    ball.draw(window);
    paddle.draw(window);
    for (const auto& block : blocks)
    {
        block->draw(window);
    }
}

void Game::handleInput(sf::Keyboard::Key key)
{
}

void Game::initializeBlocks()
{
    blocks.clear();
    
    const int ROWS = 3;
    const int COLS = 8;
    const float BLOCK_WIDTH = 2 * CELL_SIZE;
    const float BLOCK_HEIGHT = CELL_SIZE;
    const float START_Y = 80.0f;
    
    float totalWidth = COLS * BLOCK_WIDTH;
    float startX = (SCREEN_WIDTH - totalWidth) / 2.0f;
    
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            float x = startX + col * BLOCK_WIDTH;
            float y = START_Y + row * BLOCK_HEIGHT;
            sf::Vector2f position(x, y);
            
            if (row == 0)
            {
                // Нижний ряд: блоки, которые ломаются с 3 ударов
                blocks.push_back(std::make_unique<TripleBlock>(position));
            }
            else if (row == 1)
            {
                // Второй ряд: по 2 неубиваемых блока слева и справа, остальные - крепкие (2 удара)
                if (col < 2 || col >= COLS - 2)
                    blocks.push_back(std::make_unique<UnbreakableBlock>(position));
                else
                    blocks.push_back(std::make_unique<StrongBlock>(position, 2));
            }
            else if (row == 2)
            {
                // Верхний ряд: обычные блоки (ломаются с 1 удара)
                blocks.push_back(std::make_unique<SimpleBlock>(position));
            }
        }
    }
}

void Game::restartGame()
{
    ball = Ball();
    paddle = Paddle();
    initializeBlocks();
    score = 0;
    isInRelativeMotionZone = false;
    hasSideCollision = false;
    sideCollisionDelayFrames = 0;
}

void Game::checkRelativeMotionAndCollision(float deltaTime)
{
    checkBallPaddleCollision();
}

void Game::checkBallPaddleCollision()
{
    // Определение стороны столкновения по перекрытиям (ширина vs высота)
    sf::FloatRect ballBounds = ball.getBounds();
    sf::FloatRect paddleBounds = paddle.getBounds();
    
    if (ballBounds.intersects(paddleBounds))
    {
        if (soundManager)
            soundManager->playAppleEat();
            
        float ballRight = ballBounds.left + ballBounds.width;
        float ballBottom = ballBounds.top + ballBounds.height;
        
        float paddleLeft = paddleBounds.left;
        float paddleRight = paddleBounds.left + paddleBounds.width;
        float paddleTop = paddleBounds.top;
        float paddleBottom = paddleBounds.top + paddleBounds.height;
        
        // Вычисление перекрытий
        float overlapWidth = 0.0f;
        if (ballRight > paddleLeft && ballRight < paddleRight)
        {
            overlapWidth = ballRight - paddleLeft;
        }
        else if (ballRight >= paddleRight)
        {
            overlapWidth = paddleRight - (ballRight - ballBounds.width);
        }
        
        float overlapHeight = 0.0f;
        if (ballBottom > paddleTop && ballBottom < paddleBottom)
        {
            overlapHeight = ballBottom - paddleTop;
        }
        else if (ballBottom >= paddleBottom)
        {
            overlapHeight = paddleBottom - (ballBottom - ballBounds.height);
        }
        
        // Определение стороны столкновения и применение импульса
        if (overlapWidth < overlapHeight)
        {
            // Боковое столкновение
            hasSideCollision = true;
            sideCollisionDelayFrames = 0;
            ball.reflectX();
            
            // Применение импульса с учетом направления движения платформы
            sf::Vector2f ballVelocity = ball.getVelocity();
            float paddleVelocityX = paddle.getVelocityX();
            float velocityProduct = paddleVelocityX * ballVelocity.x;
            
            if (velocityProduct > 0.0f)  // Одно направление
            {
                float impulse = std::abs(paddleVelocityX) * PADDLE_IMPULSE_COEFFICIENT;
                if (paddleVelocityX > 0.0f)
                    ballVelocity.x += impulse;
                else
                    ballVelocity.x -= impulse;
                ball.setVelocityX(ballVelocity.x);
            }
            else if (velocityProduct < 0.0f)
            {
                ballVelocity.x -= paddleVelocityX;
                ball.setVelocityX(ballVelocity.x);
            }
        }
        else
        {
            // Вертикальное столкновение (сверху)
            ball.reflectY();
        }
    }
}

void Game::restoreBallSpeedToInitial()
{
    // Восстановление скорости по X, сохранение скорости по Y
    sf::Vector2f currentVelocity = ball.getVelocity();
    float preservedY = currentVelocity.y;
    float currentSpeedX = std::abs(currentVelocity.x);
    
    if (currentSpeedX > 0.0f)
    {
        float normalizedX = currentVelocity.x / currentSpeedX;
        float restoredX = normalizedX * (INITIAL_BALL_SPEED * SPEED_X_RESTORE_RATIO);
        ball.setVelocity(sf::Vector2f(restoredX, preservedY));
    }
    else
    {
        ball.setVelocity(sf::Vector2f(0.0f, preservedY));
    }
}

void Game::checkBallScreenCollisions()
{
    sf::FloatRect ballBounds = ball.getBounds();
    
    if (ballBounds.left < 0)
    {
        ball.reflectX();
        if (soundManager)
            soundManager->playAppleEat();
    }
    
    if (ballBounds.left + ballBounds.width > SCREEN_WIDTH)
    {
        ball.reflectX();
        if (soundManager)
            soundManager->playAppleEat();
    }
    
    if (ballBounds.top < 0)
    {
        ball.reflectY();
        if (soundManager)
            soundManager->playAppleEat();
    }
}

void Game::checkBallBlocksCollisions(float deltaTime)
{
    // Определение стороны столкновения по диапазонам X (метод deltaX)
    sf::FloatRect ballBounds = ball.getBounds();
    sf::Vector2f velocity = ball.getVelocity();
    float deltaX = deltaTime * velocity.x;
    float epsilon = 0.1f;
    float X = ballBounds.left + ballBounds.width;
    float D = ballBounds.width;
    
    for (auto it = blocks.begin(); it != blocks.end();)
    {
        if ((*it)->isDestroyed())
        {
            ++it;
            continue;
        }
        
        sf::FloatRect blockBounds = (*it)->getBounds();
        
        if (ballBounds.intersects(blockBounds))
        {
            if (soundManager)
                soundManager->playAppleEat();
            
            // Упрощенный метод определения стороны столкновения (только правая и нижняя граница шарика)
            float ballRight = ballBounds.left + ballBounds.width;
            float ballBottom = ballBounds.top + ballBounds.height;
            
            float blockLeft = blockBounds.left;
            float blockRight = blockBounds.left + blockBounds.width;
            float blockTop = blockBounds.top;
            float blockBottom = blockBounds.top + blockBounds.height;
            
            // Вычисление перекрытий (упрощенный способ)
            float overlapWidth = 0.0f;
            if (ballRight > blockLeft && ballRight < blockRight)
            {
                overlapWidth = ballRight - blockLeft;
            }
            else if (ballRight >= blockRight)
            {
                overlapWidth = blockRight - (ballRight - ballBounds.width);
            }
            
            float overlapHeight = 0.0f;
            if (ballBottom > blockTop && ballBottom < blockBottom)
            {
                overlapHeight = ballBottom - blockTop;
            }
            else if (ballBottom >= blockBottom)
            {
                overlapHeight = blockBottom - (ballBottom - ballBounds.height);
            }
            
            // Определение стороны столкновения по минимальному перекрытию
            if (overlapWidth < overlapHeight)
            {
                // Боковое столкновение
                ball.reflectX();
            }
            else
            {
                // Вертикальное столкновение
                ball.reflectY();
            }
            
            (*it)->onHit();
            
            if ((*it)->isDestroyed())
            {
                score += 10;
                it = blocks.erase(it);
            }
            else
            {
                ++it;
            }
            
            break;
        }
        else
        {
            ++it;
        }
    }
}

bool Game::isGameOver() const
{
    sf::FloatRect ballBounds = ball.getBounds();
    return (ballBounds.top + ballBounds.height > SCREEN_HEIGHT);
}

bool Game::isVictory() const
{
    return blocks.empty();
}