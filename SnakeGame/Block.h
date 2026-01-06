#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameObject.h"

namespace ArkanoidGame
{

enum class BlockType
{
    SIMPLE,
    STRONG,
    TRIPLE,
    UNBREAKABLE,
    BONUS
};

class Block : public GameObject
{
public:
    Block(const sf::Vector2f& position);
    virtual ~Block() = default;
    
    void update(float deltaTime, const sf::RenderWindow* window = nullptr) override;
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    
    virtual void onHit() = 0;
    virtual bool isDestroyed() const = 0;
    virtual int getHitsRemaining() const = 0;
    virtual int getPoints() const = 0;

protected:
    sf::RectangleShape shape;
    float width;
    float height;

    void initializeShape();
};

// Обычный блок - уничтожается с одного удара
class SimpleBlock : public Block
{
public:
    SimpleBlock(const sf::Vector2f& position);
    void onHit() override;
    bool isDestroyed() const override;
    int getHitsRemaining() const override;
    int getPoints() const override;

private:
    bool destroyed;
};

// Прочный блок - требует несколько ударов
class StrongBlock : public Block
{
public:
    StrongBlock(const sf::Vector2f& position, int hitsRequired = 2);
    void onHit() override;
    bool isDestroyed() const override;
    int getHitsRemaining() const override;
    int getPoints() const override;

private:
    int hitsRequired;
    int currentHits;
};

// Блок с бонусом
class BonusBlock : public Block
{
public:
    BonusBlock(const sf::Vector2f& position);
    void onHit() override;
    bool isDestroyed() const override;
    int getHitsRemaining() const override;
    int getPoints() const override;

private:
    bool destroyed;
};

// Неубиваемый блок
class UnbreakableBlock : public Block
{
public:
    UnbreakableBlock(const sf::Vector2f& position);
    void onHit() override;
    bool isDestroyed() const override;
    int getHitsRemaining() const override;
    int getPoints() const override;
    void destroyByArmor(); // Метод для уничтожения бронебойным шариком

private:
    bool destroyedByArmor = false; // Флаг уничтожения бронебойным шариком
};

// Блок с тремя ударами
class TripleBlock : public Block
{
public:
    TripleBlock(const sf::Vector2f& position);
    void onHit() override;
    bool isDestroyed() const override;
    int getHitsRemaining() const override;
    int getPoints() const override;

private:
    int currentHits;
    void updateColor();
};

} // namespace ArkanoidGame

