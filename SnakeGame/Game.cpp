#include "Game.h"
#include "Constants.h"
#include "SoundManager.h"
#include "GameObjectFactory.h"
#include <cmath> // For std::abs, std::sqrt
#include <cstdlib> // For rand()

using namespace ArkanoidGame;

void Game::initialize(SoundManager* soundManager)
{
    this->soundManager = soundManager;
    score = 0;
    initializeBlocks();
}

void Game::update(float deltaTime, const sf::RenderWindow* window)
{
    ball.update(deltaTime);
    paddle.update(deltaTime, window);
    checkBallScreenCollisions();
    checkBallPaddleCollision();
    checkBallBlocksCollisions(deltaTime);
    
    // Обновление бонусов
    updateBonuses(deltaTime);
    checkBonusCollection();
    
    // Обновление активного эффекта
    if (currentEffect && currentEffect->isActive())
    {
        currentEffect->update(deltaTime);
        if (!currentEffect->isActive())
        {
            currentEffect->remove(&ball, &paddle);
            currentEffect.reset();
        }
    }
    
    // Восстановление скорости после бокового столкновения (через задержку)
    if (hasSideCollision)
    {
        sf::FloatRect ballBounds = ball.getBounds();
        sf::FloatRect paddleBounds = paddle.getBounds();
        
        if (!ballBounds.intersects(paddleBounds))
        {
            // Ограничение скорости в течение 2 кадров
            sf::Vector2f ballVelocity = ball.getVelocity();
            float currentSpeed = std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y);
            if (currentSpeed > MAX_BALL_SPEED)
            {
                float scale = MAX_BALL_SPEED / currentSpeed;
                ballVelocity.x *= scale;
                ballVelocity.y *= scale;
                ball.setVelocity(ballVelocity);
            }
            
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
        if (!block->isDestroyed())
            block->draw(window);
    }
    
    // Отрисовка падающих бонусов
    for (const auto& bonus : fallingBonuses)
    {
        bonus->draw(window);
    }
}

void Game::handleInput(sf::Keyboard::Key key)
{
}

void Game::initializeBlocks()
{
    blocks.clear();
    
    const int ROWS = BLOCKS_ROWS;
    const int COLS = BLOCKS_COLS;
    const float BLOCK_WIDTH = 2 * CELL_SIZE;
    const float BLOCK_HEIGHT = CELL_SIZE;
    const float START_Y = BLOCKS_START_Y;
    
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
                blocks.push_back(GameObjectFactory::createBlock(BlockType::TRIPLE, position));
            }
            else if (row == 1)
            {
                // Второй ряд: по 2 неубиваемых блока слева и справа, остальные - крепкие (2 удара)
                if (col < UNBREAKABLE_BLOCKS_PER_SIDE || col >= COLS - UNBREAKABLE_BLOCKS_PER_SIDE)
                    blocks.push_back(GameObjectFactory::createBlock(BlockType::UNBREAKABLE, position));
                else
                    blocks.push_back(GameObjectFactory::createBlock(BlockType::STRONG, position));
            }
            else if (row == 2)
            {
                // Верхний ряд: обычные блоки (ломаются с 1 удара)
                blocks.push_back(GameObjectFactory::createBlock(BlockType::SIMPLE, position));
            }
        }
    }
}

void Game::restartGame()
{
    // Сброс жизней при полном перезапуске
    lives = INITIAL_LIVES;
    
    ball = Ball();
    paddle = Paddle();
    initializeBlocks();
    score = 0;
    hasSideCollision = false;
    sideCollisionDelayFrames = 0;
    
    // Очистка бонусов и эффектов
    fallingBonuses.clear();
    if (currentEffect)
    {
        currentEffect->remove(&ball, &paddle);
        currentEffect.reset();
    }
}

void Game::checkBallPaddleCollision()
{
    // Комбинированный метод: перекрытия для типа столкновения + центры для конкретной стороны
    sf::FloatRect ballBounds = ball.getBounds();
    sf::FloatRect paddleBounds = paddle.getBounds();
    
    if (ballBounds.intersects(paddleBounds))
    {
        if (soundManager)
            soundManager->playBlockHit();
            
        // Стандартный метод определения стороны столкновения через перекрытия (как для блоков)
        float ballLeft = ballBounds.left;
        float ballRight = ballBounds.left + ballBounds.width;
        float ballTop = ballBounds.top;
        float ballBottom = ballBounds.top + ballBounds.height;
        float paddleLeft = paddleBounds.left;
        float paddleRight = paddleBounds.left + paddleBounds.width;
        float paddleTop = paddleBounds.top;
        float paddleBottom = paddleBounds.top + paddleBounds.height;
        
        // Вычисляем перекрытие по X (без min/max)
        float overlapX = 0.0f;
        float overlapStartX = ballLeft;
        float overlapEndX = ballRight;
        if (paddleLeft > overlapStartX) overlapStartX = paddleLeft;
        if (paddleRight < overlapEndX) overlapEndX = paddleRight;
        if (overlapEndX > overlapStartX) overlapX = overlapEndX - overlapStartX;
        
        // Вычисляем перекрытие по Y (без min/max)
        float overlapY = 0.0f;
        float overlapStartY = ballTop;
        float overlapEndY = ballBottom;
        if (paddleTop > overlapStartY) overlapStartY = paddleTop;
        if (paddleBottom < overlapEndY) overlapEndY = paddleBottom;
        if (overlapEndY > overlapStartY) overlapY = overlapEndY - overlapStartY;
        
        // Определяем сторону столкновения по минимальному перекрытию (без min/max)
        if (overlapX < overlapY)
        {
            // Боковое столкновение
            hasSideCollision = true;
            sideCollisionDelayFrames = 0;
            
            // Контроль знака: определяем направление в зависимости от позиции мяча относительно платформы
            sf::Vector2f ballVelocity = ball.getVelocity();
            float ballCenterX = ballBounds.left + ballBounds.width / 2.0f;
            float paddleCenterX = paddleBounds.left + paddleBounds.width / 2.0f;
            
            float speedX = std::abs(ballVelocity.x);
            if (ballCenterX < paddleCenterX)
            {
                // Столкновение слева - явно влево (отрицательное значение)
                ballVelocity.x = -speedX;
            }
            else
            {
                // Столкновение справа - явно вправо (положительное значение)
                ballVelocity.x = speedX;
            }
            
            // Устанавливаем скорость перед применением импульса
            ball.setVelocity(ballVelocity);
            
            // Применение импульса с учетом направления движения платформы
            float paddleVelocityX = paddle.getVelocityX();
            float velocityProduct = paddleVelocityX * ballVelocity.x;
            
            if (velocityProduct > 0.0f)  // Одно направление
            {
                float impulse = std::abs(paddleVelocityX) * PADDLE_IMPULSE_COEFFICIENT;
                if (paddleVelocityX > 0.0f)
                    ballVelocity.x += impulse;
                else
                    ballVelocity.x -= impulse;
                
                // Ограничение общей скорости (вектор)
                float currentSpeed = std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y);
                if (currentSpeed > MAX_BALL_SPEED)
                {
                    float scale = MAX_BALL_SPEED / currentSpeed;
                    ballVelocity.x *= scale;
                    ballVelocity.y *= scale;
                }
                
                ball.setVelocity(ballVelocity);
            }
            else if (velocityProduct < 0.0f)
            {
                ballVelocity.x -= paddleVelocityX;
                
                // Ограничение общей скорости (вектор)
                float currentSpeed = std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y);
                if (currentSpeed > MAX_BALL_SPEED)
                {
                    float scale = MAX_BALL_SPEED / currentSpeed;
                    ballVelocity.x *= scale;
                    ballVelocity.y *= scale;
                }
                
                ball.setVelocity(ballVelocity);
            }
        }
        else
        {
            // Вертикальное столкновение (сверху платформы)
            // Контроль знака: явно устанавливаем движение вверх после отражения от платформы
            sf::Vector2f ballVelocity = ball.getVelocity();
            float speedY = std::abs(ballVelocity.y);
            ballVelocity.y = -speedY;  // Явно вверх (отрицательное значение)
            ball.setVelocity(ballVelocity);
        }
    }
}

void Game::checkBallScreenCollisions()
{
    sf::FloatRect ballBounds = ball.getBounds();
    sf::Vector2f ballVelocity = ball.getVelocity();
    
    if (ballBounds.left < 0)
    {
        // Контроль знака: шарик ударился о левую стенку, явно устанавливаем движение вправо
        float speedX = std::abs(ballVelocity.x);
        ball.setVelocityX(speedX);  // Явно вправо (положительное значение)
        if (soundManager)
            soundManager->playBlockHit();
    }
    
    if (ballBounds.left + ballBounds.width > SCREEN_WIDTH)
    {
        // Контроль знака: шарик ударился о правую стенку, явно устанавливаем движение влево
        float speedX = std::abs(ballVelocity.x);
        ball.setVelocityX(-speedX);  // Явно влево (отрицательное значение)
        if (soundManager)
            soundManager->playBlockHit();
    }
    
    if (ballBounds.top < 0)
    {
        // Контроль знака: шарик ударился о верхнюю стенку, явно устанавливаем движение вниз
        float speedY = std::abs(ballVelocity.y);
        ballVelocity.y = speedY;  // Явно вниз (положительное значение)
        ball.setVelocity(ballVelocity);
        if (soundManager)
            soundManager->playBlockHit();
    }
}

void Game::checkBallBlocksCollisions(float deltaTime)
{
    sf::FloatRect ballBounds = ball.getBounds();
    
    for (auto it = blocks.begin(); it != blocks.end(); ++it)
    {
        if ((*it)->isDestroyed())
            continue;
        
        sf::FloatRect blockBounds = (*it)->getBounds();
        
        if (ballBounds.intersects(blockBounds))
        {
            if (soundManager)
                soundManager->playBlockHit();
            
            // Стандартный метод определения стороны столкновения через перекрытия
            float ballLeft = ballBounds.left;
            float ballRight = ballBounds.left + ballBounds.width;
            float ballTop = ballBounds.top;
            float ballBottom = ballBounds.top + ballBounds.height;
            float blockLeft = blockBounds.left;
            float blockRight = blockBounds.left + blockBounds.width;
            float blockTop = blockBounds.top;
            float blockBottom = blockBounds.top + blockBounds.height;
            
            // Вычисляем перекрытие по X (без min/max)
            float overlapX = 0.0f;
            float overlapStartX = ballLeft;
            float overlapEndX = ballRight;
            if (blockLeft > overlapStartX) overlapStartX = blockLeft;
            if (blockRight < overlapEndX) overlapEndX = blockRight;
            if (overlapEndX > overlapStartX) overlapX = overlapEndX - overlapStartX;
            
            // Вычисляем перекрытие по Y (без min/max)
            float overlapY = 0.0f;
            float overlapStartY = ballTop;
            float overlapEndY = ballBottom;
            if (blockTop > overlapStartY) overlapStartY = blockTop;
            if (blockBottom < overlapEndY) overlapEndY = blockBottom;
            if (overlapEndY > overlapStartY) overlapY = overlapEndY - overlapStartY;
            
            // Определяем сторону столкновения по минимальному перекрытию (без min/max)
            sf::Vector2f ballVelocity = ball.getVelocity();
            
            if (overlapX < overlapY)
            {
                // Боковое столкновение
                // Контроль знака: определяем направление в зависимости от позиции мяча относительно блока
                float ballCenterX = ballBounds.left + ballBounds.width / 2.0f;
                float blockCenterX = blockBounds.left + blockBounds.width / 2.0f;
                
                float speedX = std::abs(ballVelocity.x);
                if (ballCenterX < blockCenterX)
                {
                    // Столкновение слева - явно влево (отрицательное значение)
                    ballVelocity.x = -speedX;
                }
                else
                {
                    // Столкновение справа - явно вправо (положительное значение)
                    ballVelocity.x = speedX;
                }
            }
            else
            {
                // Вертикальное столкновение
                // Контроль знака: определяем направление в зависимости от позиции мяча относительно блока
                float ballCenterY = ballBounds.top + ballBounds.height / 2.0f;
                float blockCenterY = blockBounds.top + blockBounds.height / 2.0f;
                
                float speedY = std::abs(ballVelocity.y);
                if (ballCenterY < blockCenterY)
                {
                    // Столкновение сверху - явно вверх (отрицательное значение)
                    ballVelocity.y = -speedY;
                }
                else
                {
                    // Столкновение снизу - явно вниз (положительное значение)
                    ballVelocity.y = speedY;
                }
            }
            
            // Проверка бронебойности
            bool isArmorActive = ball.isArmorActive();
            bool blockWasDestroyed = (*it)->isDestroyed();
            
            if (!isArmorActive)
            {
                // Обычное столкновение - применяем отражение
                ball.setVelocity(ballVelocity);
            }
            
            // Если шарик бронебойный, уничтожаем блок (включая неубиваемые)
            if (isArmorActive)
            {
                UnbreakableBlock* unbreakable = dynamic_cast<UnbreakableBlock*>(it->get());
                if (unbreakable)
                {
                    unbreakable->destroyByArmor(); // Уничтожаем неубиваемый блок бронебойным шариком
                }
                else
                {
                    (*it)->onHit(); // Обычные блоки уничтожаются через onHit()
                }
            }
            else
            {
                (*it)->onHit(); // Обычное столкновение
            }
            
            // Автосохранение при попадании в блок
            lastSnapshot = createSnapshot();
            
            bool blockIsDestroyed = (*it)->isDestroyed();
            
            // Если блок был уничтожен (обычным способом или бронебойным)
            if (blockIsDestroyed && !blockWasDestroyed)
            {
                // Начисление очков через getPoints()
                // Для UnbreakableBlock даем 4 очка только если уничтожен бронебойным шариком
                UnbreakableBlock* unbreakable = dynamic_cast<UnbreakableBlock*>(it->get());
                if (unbreakable && isArmorActive)
                {
                    score += (*it)->getPoints(); // 4 очка за UnbreakableBlock при бронебойности
                }
                else if (!unbreakable)
                {
                    score += (*it)->getPoints(); // Обычные блоки: 1, 2 или 3 очка
                }
                // UnbreakableBlock, уничтоженный обычным способом, не дает очков
                
                // Спавн бонуса с вероятностью BONUS_SPAWN_CHANCE_PERCENT%
                if ((rand() % 100) < BONUS_SPAWN_CHANCE_PERCENT)
                {
                    sf::Vector2f blockCenter(
                        blockBounds.left + blockBounds.width / 2.0f,
                        blockBounds.top + blockBounds.height / 2.0f
                    );
                    spawnBonus(blockCenter);
                }
            }
            
            // Если шарик бронебойный, не отражаемся (пролетаем насквозь)
            if (!isArmorActive)
            {
                break;
            }
        }
    }
}

bool Game::isBallFell() const
{
    sf::FloatRect ballBounds = ball.getBounds();
    return (ballBounds.top + ballBounds.height > SCREEN_HEIGHT);
}

bool Game::isGameOver() const
{
    return (lives == 0);
}

bool Game::isVictory() const
{
    for (const auto& block : blocks)
    {
        // Пропускаем неубиваемые блоки - они не учитываются при проверке победы
        UnbreakableBlock* unbreakable = dynamic_cast<UnbreakableBlock*>(block.get());
        if (unbreakable)
            continue;
        
        if (!block->isDestroyed())
            return false;
    }
    return true;
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

void Game::spawnBonus(const sf::Vector2f& position)
{
    // Случайный тип бонуса
    BonusType type = static_cast<BonusType>(rand() % 3);
    fallingBonuses.push_back(GameObjectFactory::createBonus(type, position));
}

void Game::updateBonuses(float deltaTime)
{
    for (auto it = fallingBonuses.begin(); it != fallingBonuses.end();)
    {
        (*it)->update(deltaTime, nullptr);
        
        // Удаляем бонусы, которые улетели за экран
        sf::FloatRect bonusBounds = (*it)->getBounds();
        if (bonusBounds.top > SCREEN_HEIGHT)
        {
            it = fallingBonuses.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::checkBonusCollection()
{
    sf::FloatRect paddleBounds = paddle.getBounds();
    
    for (auto it = fallingBonuses.begin(); it != fallingBonuses.end();)
    {
        sf::FloatRect bonusBounds = (*it)->getBounds();
        
        if (bonusBounds.intersects(paddleBounds))
        {
            // Собираем бонус
            applyBonusEffect((*it)->getType());
            it = fallingBonuses.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::applyBonusEffect(BonusType bonusType)
{
    // Снимаем старый эффект, если есть
    if (currentEffect)
    {
        currentEffect->remove(&ball, &paddle);
        currentEffect.reset();
    }
    
    // Создаем и применяем новый эффект
    switch (bonusType)
    {
    case BonusType::PADDLE_SIZE:
        currentEffect = std::make_unique<PaddleSizeEffect>();
        break;
    case BonusType::SPEED:
        currentEffect = std::make_unique<SpeedBallEffect>();
        break;
    case BonusType::ARMOR:
        currentEffect = std::make_unique<ArmorBallEffect>();
        break;
    }
    
    if (currentEffect)
    {
        currentEffect->apply(&ball, &paddle);
    }
}

BlockType Game::getBlockType(Block* block) const
{
    if (dynamic_cast<SimpleBlock*>(block))
        return BlockType::SIMPLE;
    else if (dynamic_cast<StrongBlock*>(block))
        return BlockType::STRONG;
    else if (dynamic_cast<TripleBlock*>(block))
        return BlockType::TRIPLE;
    else if (dynamic_cast<UnbreakableBlock*>(block))
        return BlockType::UNBREAKABLE;
    else if (dynamic_cast<BonusBlock*>(block))
        return BlockType::BONUS;
    return BlockType::SIMPLE; // По умолчанию
}

GameStateSnapshot Game::createSnapshot() const
{
    GameStateSnapshot snapshot;
    snapshot.setScore(score);
    
    // Сохраняем состояние всех блоков
    for (const auto& block : blocks)
    {
        if (!block->isDestroyed())
        {
            BlockState blockState;
            blockState.type = getBlockType(block.get());
            blockState.position = block->getPosition();
            blockState.hitsRemaining = block->getHitsRemaining();
            snapshot.addBlockState(blockState);
        }
    }
    
    return snapshot;
}

void Game::restoreFromSnapshot(const GameStateSnapshot& snapshot)
{
    // Восстанавливаем очки
    score = snapshot.getScore();
    
    // Очищаем текущие блоки
    blocks.clear();
    
    // Восстанавливаем блоки из snapshot
    const auto& savedBlocks = snapshot.getBlocks();
    for (const auto& blockState : savedBlocks)
    {
        std::unique_ptr<Block> block = GameObjectFactory::createBlock(blockState.type, blockState.position);
        
        // Восстанавливаем состояние блока (количество ударов)
        if (blockState.hitsRemaining > 0)
        {
            StrongBlock* strongBlock = dynamic_cast<StrongBlock*>(block.get());
            if (strongBlock)
            {
                int hitsRequired = STRONG_BLOCK_HITS_REQUIRED;
                int hitsTaken = hitsRequired - blockState.hitsRemaining;
                for (int i = 0; i < hitsTaken; i++)
                {
                    strongBlock->onHit();
                }
            }
            
            TripleBlock* tripleBlock = dynamic_cast<TripleBlock*>(block.get());
            if (tripleBlock)
            {
                int hitsTaken = TRIPLE_BLOCK_HITS_REQUIRED - blockState.hitsRemaining;
                for (int i = 0; i < hitsTaken; i++)
                {
                    tripleBlock->onHit();
                }
            }
        }
        
        blocks.push_back(std::move(block));
    }
    
    // Шарик и платформа возвращаются в начальные позиции (не из snapshot)
    ball = Ball();
    paddle = Paddle();
    
    // Очищаем бонусы и эффекты
    fallingBonuses.clear();
    if (currentEffect)
    {
        currentEffect->remove(&ball, &paddle);
        currentEffect.reset();
    }
    
    // Сброс флагов столкновений
    hasSideCollision = false;
    sideCollisionDelayFrames = 0;
}

void Game::loseLife()
{
    lives--;
    if (lives > 0)
    {
        // Автозагрузка из последнего сохранения
        restoreFromSnapshot(lastSnapshot);
    }
    // Если lives == 0, игра закончится (проверка в isGameOver())
}
