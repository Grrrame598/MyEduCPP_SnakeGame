#include "BonusEffect.h"
#include "Ball.h"
#include "Paddle.h"
#include "Constants.h"
#include <cmath>

namespace ArkanoidGame
{

BonusEffect::BonusEffect(float duration)
    : duration(duration),
      remainingTime(duration),
      active(false)
{
}

void BonusEffect::update(float deltaTime)
{
    if (active)
    {
        remainingTime -= deltaTime;
        if (remainingTime <= 0.0f)
        {
            active = false;
            remainingTime = 0.0f;
        }
    }
}

PaddleSizeEffect::PaddleSizeEffect()
    : BonusEffect(5.0f),
      originalWidth(0.0f),
      multiplier(2.0f)
{
}

void PaddleSizeEffect::apply(Ball* ball, Paddle* paddle)
{
    (void)ball;
    
    if (paddle)
    {
        originalWidth = paddle->getSize().x;
        paddle->setWidth(originalWidth * multiplier);
        active = true;
        remainingTime = duration;
    }
}

void PaddleSizeEffect::remove(Ball* ball, Paddle* paddle)
{
    (void)ball;
    
    if (paddle && originalWidth > 0.0f)
    {
        paddle->setWidth(originalWidth);
        active = false;
        originalWidth = 0.0f;
    }
}

SpeedBallEffect::SpeedBallEffect()
    : BonusEffect(5.0f),
      originalSpeed(0.0f),
      originalColor(sf::Color::White),
      speedMultiplier(1.5f)
{
}

void SpeedBallEffect::apply(Ball* ball, Paddle* paddle)
{
    (void)paddle;
    
    if (ball)
    {
        sf::Vector2f v = ball->getVelocity();
        originalSpeed = std::sqrt(v.x * v.x + v.y * v.y);
        originalColor = ball->getColor();
        
        if (originalSpeed > 0.0f)
        {
            float newSpeed = originalSpeed * speedMultiplier;
            sf::Vector2f direction(v.x / originalSpeed, v.y / originalSpeed);
            ball->setVelocity(direction * newSpeed);
        }
        
        ball->setColor(sf::Color::Green);
        active = true;
        remainingTime = duration;
    }
}

void SpeedBallEffect::remove(Ball* ball, Paddle* paddle)
{
    (void)paddle;
    
    if (ball && originalSpeed > 0.0f)
    {
        sf::Vector2f v = ball->getVelocity();
        float currentSpeed = std::sqrt(v.x * v.x + v.y * v.y);
        
        if (currentSpeed > 0.0f)
        {
            sf::Vector2f direction(v.x / currentSpeed, v.y / currentSpeed);
            ball->setVelocity(direction * originalSpeed);
        }
        
        ball->setColor(originalColor);
        active = false;
    }
}

ArmorBallEffect::ArmorBallEffect()
    : BonusEffect(5.0f),
      armorActive(false),
      originalColor(sf::Color::White)
{
}

void ArmorBallEffect::apply(Ball* ball, Paddle* paddle)
{
    (void)paddle;
    
    if (ball)
    {
        originalColor = ball->getColor();
        ball->setColor(sf::Color(255, 215, 0));
        ball->setArmorActive(true);
        armorActive = true;
        active = true;
        remainingTime = duration;
    }
}

void ArmorBallEffect::remove(Ball* ball, Paddle* paddle)
{
    (void)paddle;
    
    if (ball)
    {
        ball->setColor(originalColor);
        ball->setArmorActive(false);
        armorActive = false;
        active = false;
    }
}

} // namespace ArkanoidGame


