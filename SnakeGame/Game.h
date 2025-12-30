#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Application.h"
#include "Block.h"
#include "Ball.h"
#include "Paddle.h"

namespace SnakeGame
{

class SoundManager;

class Game : public Application
{
public:
    void initialize(int P, int V, int L, SoundManager* soundManager);
    void update(float deltaTime, const sf::RenderWindow* window = nullptr) override;
    void draw(sf::RenderWindow& window) override;
    void handleInput(sf::Keyboard::Key key) override;
    int getNumEatenApples() const { return numEatenApples; }
    int getScore() const { return score; }
    bool isGameOver() const;  // Проверка: шарик упал за нижнюю границу
    bool isVictory() const;   // Проверка: все блоки уничтожены
    void restartGame();       // Полный перезапуск игры
    
private:
    Ball ball;
    Paddle paddle;
    std::vector<std::unique_ptr<Block>> blocks;
    
    int numEatenApples = 0;
    int pointsPerApple = 2;
    int gameSpeed = 0;
    int growthLength = 0;
    SoundManager* soundManager = nullptr;
    int score = 0;
    
    bool isInRelativeMotionZone = false;
    sf::Vector2f savedAbsoluteVelocity;
    bool hasSideCollision = false;              // Флаг бокового столкновения с платформой
    int sideCollisionDelayFrames = 0;           // Задержка перед восстановлением скорости
    
    static constexpr float PADDLE_IMPULSE_COEFFICIENT = 20.0f;  // Коэффициент импульса при боковом столкновении
    static constexpr int SPEED_RESTORE_DELAY_FRAMES = 2;        // Задержка в кадрах перед восстановлением скорости
    static constexpr float INITIAL_BALL_SPEED = 300.0f;         // Начальная скорость шарика
    static constexpr float SPEED_X_RESTORE_RATIO = 0.5f;        // Коэффициент восстановления скорости по X
    
    void initializeBlocks();                    // Инициализация блоков на поле
    void checkRelativeMotionAndCollision(float deltaTime);
    void checkBallPaddleCollision();            // Проверка столкновения шарика с платформой
    void restoreBallSpeedToInitial();           // Восстановление скорости шарика к начальной
    void checkBallScreenCollisions();           // Проверка столкновений с границами экрана
    void checkBallBlocksCollisions(float deltaTime);  // Проверка столкновений с блоками
};

} // namespace SnakeGame