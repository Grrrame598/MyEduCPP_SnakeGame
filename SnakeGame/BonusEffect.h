#pragma once
#include <SFML/Graphics.hpp>

namespace ArkanoidGame
{

class Ball;
class Paddle;

class BonusEffect
{
public:
    BonusEffect(float duration);
    virtual ~BonusEffect() = default;
    
    virtual void apply(Ball* ball, Paddle* paddle) = 0;
    virtual void update(float deltaTime);
    virtual void remove(Ball* ball, Paddle* paddle) = 0;
    
    bool isActive() const { return active && remainingTime > 0.0f; }
    float getRemainingTime() const { return remainingTime; }

protected:
    float duration;
    float remainingTime;
    bool active;
};

class PaddleSizeEffect : public BonusEffect
{
public:
    PaddleSizeEffect();
    void apply(Ball* ball, Paddle* paddle) override;
    void remove(Ball* ball, Paddle* paddle) override;

private:
    float originalWidth;
    float multiplier;
};

class SpeedBallEffect : public BonusEffect
{
public:
    SpeedBallEffect();
    void apply(Ball* ball, Paddle* paddle) override;
    void remove(Ball* ball, Paddle* paddle) override;

private:
    float originalSpeed;
    sf::Color originalColor;
    float speedMultiplier;
};

class ArmorBallEffect : public BonusEffect
{
public:
    ArmorBallEffect();
    void apply(Ball* ball, Paddle* paddle) override;
    void remove(Ball* ball, Paddle* paddle) override;
    
    bool isArmorActive() const { return armorActive; }

private:
    bool armorActive;
    sf::Color originalColor;
};

} // namespace ArkanoidGame

