#include "Snake.h"
#include "Constants.h"
#include "Math.h"
#include <cassert>

namespace SnakeGame
{

Snake::Snake()
{
    currentDirection = Direction::RIGHT;
    loadTextures();
}

void Snake::loadTextures()
{
    assert(headTextures[0].loadFromFile(RESOURCES_PATH + "head_up.png"));
    assert(headTextures[1].loadFromFile(RESOURCES_PATH + "head_down.png"));
    assert(headTextures[2].loadFromFile(RESOURCES_PATH + "head_left.png"));
    assert(headTextures[3].loadFromFile(RESOURCES_PATH + "head_right.png"));
    
    assert(tailTextures[0].loadFromFile(RESOURCES_PATH + "tail_up.png"));
    assert(tailTextures[1].loadFromFile(RESOURCES_PATH + "tail_down.png"));
    assert(tailTextures[2].loadFromFile(RESOURCES_PATH + "tail_left.png"));
    assert(tailTextures[3].loadFromFile(RESOURCES_PATH + "tail_right.png"));
    
    assert(bodyStraightTextures[0].loadFromFile(RESOURCES_PATH + "body_vertical.png"));
    assert(bodyStraightTextures[1].loadFromFile(RESOURCES_PATH + "body_horizontal.png"));
    
    assert(bodyCornerTextures[0].loadFromFile(RESOURCES_PATH + "body_topleft.png"));
    assert(bodyCornerTextures[1].loadFromFile(RESOURCES_PATH + "body_topright.png"));
    assert(bodyCornerTextures[2].loadFromFile(RESOURCES_PATH + "body_bottomleft.png"));
    assert(bodyCornerTextures[3].loadFromFile(RESOURCES_PATH + "body_bottomright.png"));
}

void Snake::initSnake()
{
    // Очищаем тело змейки (на случай повторной инициализации)
    body.clear();
    
    // Определяем стартовую позицию головы (центр поля)
    int startX = GRID_WIDTH / 2;
    int startY = GRID_HEIGHT / 2;
    
    // Создаем 4 сегмента горизонтально, направление вправо
    // Голова (первый сегмент) - в центре
    // Остальные 3 сегмента идут влево от головы
    Segment head;
    head.position = sf::Vector2i(startX, startY);
    head.direction = Direction::RIGHT;  // Направление вправо
    body.push_front(head);
    
    // Добавляем остальные 3 сегмента со сдвигом по X влево
    for (int i = 1; i < 4; ++i)
    {
        Segment segment;
        segment.position = sf::Vector2i(startX - i, startY);
        segment.direction = Direction::RIGHT;  // Направление вправо
        body.push_back(segment);
    }
    
    // Устанавливаем начальное направление движения
    currentDirection = Direction::RIGHT;
}

void Snake::move()
{
    // Получаем позицию головы
    sf::Vector2i headPosition = body.front().position;
    // Вычисляем новую позицию головы на основе текущего направления
    switch (currentDirection)
    {
        case Direction::UP:
            headPosition.y -= 1;
            break;
        case Direction::DOWN:
            headPosition.y += 1;
            break;
        case Direction::LEFT:
            headPosition.x -= 1;
            break;
        case Direction::RIGHT:
            headPosition.x += 1;
            break;
    }
    // Создаем новый сегмент головы с текущим направлением
    Segment newHead;
    newHead.position = headPosition;
    newHead.direction = currentDirection;  // Присваиваем направление новому сегменту
    body.push_front(newHead);
    
    // Удаляем последний сегмент (хвост) - обычное движение
    body.pop_back();
}

bool Snake::checkCollisions(const sf::Vector2i& applePosition)
{
    // Проверяем столкновение с границами поля
    // Стены находятся на позициях x=1, x=GRID_WIDTH, y=1, y=GRID_HEIGHT
    // Игровая область: от 2 до GRID_WIDTH-1 (X) и от 2 до GRID_HEIGHT-1 (Y)
    if (body.front().position.x < 2 || body.front().position.x > GRID_WIDTH - 1 ||
        body.front().position.y < 2 || body.front().position.y > GRID_HEIGHT - 1)
    {
        gameState = GameState::GAME_OVER;
        return false;  // Яблоко не съедено, игра окончена
    }

    // Проверка столкновения головы с телом
    // Начинаем с 1, чтобы не проверять голову с самой собой (голова = body[0] или body.front())
    for (size_t i = 1; i < body.size(); ++i)
    {
        if (body.front().position.x == body[i].position.x && 
            body.front().position.y == body[i].position.y)
        {
            gameState = GameState::GAME_OVER;
            return false;  // Яблоко не съедено, игра окончена
        }
    }
    
    // Проверка столкновения головы с яблоком
    if (body.front().position.x == applePosition.x && 
        body.front().position.y == applePosition.y)
    {
        // Возвращаем true - яблоко съедено
        // Рост будет вызван в Game::update(), где есть доступ к параметру L
        return true;  // Яблоко съедено
    }
    
    return false;  // Яблоко не съедено
}

bool Snake::isPositionOccupied(const sf::Vector2i& pos) const
{
    // Проверяем, занята ли позиция любым сегментом змейки
    for (const auto& segment : body)
    {
        if (segment.position.x == pos.x && segment.position.y == pos.y)
        {
            return true;  // Позиция занята
        }
    }
    return false;  // Позиция свободна
}

void Snake::grow(int L)
{
    // Мгновенно добавляем L сегментов в хвост
    if (!body.empty())
    {
        // Получаем направление хвоста
        Direction tailDirection = body.back().direction;
        
        // Создаем новый сегмент с направлением хвоста
        Segment newSegment;
        newSegment.position = body.back().position;  // Позиция такая же как у хвоста
        newSegment.direction = tailDirection;  // Направление такое же как у хвоста
        
        // Добавляем L новых сегментов
        for (int i = 0; i < L; ++i)
        {
            body.push_back(newSegment);
        }
    }
}



void Snake::update(float deltaTime, int V)
{
    timeSinceLastMove += deltaTime;
    if (timeSinceLastMove >= MOVE_INTERVAL / static_cast<float>(V))
    {
        move();
        timeSinceLastMove = 0.0f;
    }
}


void Snake::setDirection(Direction newDirection)
{
    // Просто устанавливаем новое направление
    // Проверка на столкновение (включая разворот на 180°) будет в checkCollisions()
    currentDirection = newDirection;
}

void Snake::draw(sf::RenderWindow& window)
{
    if (body.empty())
        return;
    
    // Создаем спрайт локально для отрисовки (переиспользуем в цикле)
    sf::Sprite sprite;
    
    // Отрисовка всех сегментов змейки
    for (size_t i = 0; i < body.size(); ++i)
    {
        const Segment& segment = body[i];
        sf::Texture* textureToUse = nullptr;
        
        // Определяем тип сегмента
        if (i == 0)
        {
            // Голова - первый сегмент
            int directionIndex = static_cast<int>(segment.direction);
            textureToUse = &headTextures[directionIndex];
        }
        // ЗАКОММЕНТИРОВАНО: Отдельная обработка хвоста (можно вернуть при необходимости)
        // else if (i == body.size() - 1)
        // {
        //     // Хвост - последний сегмент
        //     int directionIndex = static_cast<int>(segment.direction);
        //     // Инвертируем направление для хвоста (хвост смотрит в противоположную сторону)
        //     int invertedIndex;
        //     switch (directionIndex)
        //     {
        //         case 0: invertedIndex = 1; break; // UP → DOWN
        //         case 1: invertedIndex = 0; break; // DOWN → UP
        //         case 2: invertedIndex = 3; break; // LEFT → RIGHT
        //         case 3: invertedIndex = 2; break; // RIGHT → LEFT
        //         default: invertedIndex = directionIndex; break;
        //     }
        //     textureToUse = &tailTextures[invertedIndex];
        // }
        else if (i == body.size() - 1)
        {
            // Хвост - последний сегмент, отдельная обработка
            // Вычисляем направление хвоста на основе его позиции относительно предыдущего сегмента
            // Это важно, так как direction хвоста может быть устаревшим после поворота
            sf::Vector2i tailPos = body[i].position;
            sf::Vector2i prevPos = body[i - 1].position;
            
            // Определяем направление от хвоста к предыдущему сегменту (хвост смотрит к предыдущему)
            int dx = prevPos.x - tailPos.x;
            int dy = prevPos.y - tailPos.y;
            
            Direction tailDirection;
            if (dx > 0) tailDirection = Direction::RIGHT;
            else if (dx < 0) tailDirection = Direction::LEFT;
            else if (dy > 0) tailDirection = Direction::DOWN;
            else if (dy < 0) tailDirection = Direction::UP;
            else tailDirection = Direction::RIGHT; // По умолчанию
            
            int directionIndex = static_cast<int>(tailDirection);
            // Инвертируем направление для хвоста (хвост смотрит в противоположную сторону от движения)
            int invertedIndex;
            switch (directionIndex)
            {
                case 0: invertedIndex = 1; break; // UP → DOWN
                case 1: invertedIndex = 0; break; // DOWN → UP
                case 2: invertedIndex = 3; break; // LEFT → RIGHT
                case 3: invertedIndex = 2; break; // RIGHT → LEFT
                default: invertedIndex = directionIndex; break;
            }
            textureToUse = &tailTextures[invertedIndex];
        }
        else
        {
            // Тело - промежуточные сегменты (НЕ хвост)
            Direction currentDir = segment.direction;
            Direction prevDir = body[i - 1].direction;
            
            // Проверяем, является ли это углом
            if (currentDir != prevDir)
            {
                // Определяем тип угла по комбинации предыдущего и текущего направления
                // Примечание: в системе координат Y увеличивается вниз, поэтому "top" и "bottom" в названиях файлов
                // нужно интерпретировать с учетом этой системы координат
                // Также учитываем инверсию по оси X (LEFT и RIGHT поменяны местами)
                // [0] = topleft (интерпретируем как bottomright для системы координат)
                // [1] = topright (интерпретируем как bottomleft для системы координат)
                // [2] = bottomleft (интерпретируем как topright для системы координат)
                // [3] = bottomright (интерпретируем как topleft для системы координат)
                if ((prevDir == Direction::UP && currentDir == Direction::RIGHT) ||
                    (prevDir == Direction::LEFT && currentDir == Direction::DOWN))
                {
                    textureToUse = &bodyCornerTextures[0]; // body_topleft.png (инвертировано по X)
                }
                else if ((prevDir == Direction::UP && currentDir == Direction::LEFT) ||
                         (prevDir == Direction::RIGHT && currentDir == Direction::DOWN))
                {
                    textureToUse = &bodyCornerTextures[1]; // body_topright.png (инвертировано по X)
                }
                else if ((prevDir == Direction::DOWN && currentDir == Direction::RIGHT) ||
                         (prevDir == Direction::LEFT && currentDir == Direction::UP))
                {
                    textureToUse = &bodyCornerTextures[2]; // body_bottomleft.png (инвертировано по X)
                }
                else if ((prevDir == Direction::DOWN && currentDir == Direction::LEFT) ||
                         (prevDir == Direction::RIGHT && currentDir == Direction::UP))
                {
                    textureToUse = &bodyCornerTextures[3]; // body_bottomright.png (инвертировано по X)
                }
            }
            else
            {
                // Прямой сегмент
                if (currentDir == Direction::UP || currentDir == Direction::DOWN)
                {
                    textureToUse = &bodyStraightTextures[0]; // Вертикальный
                }
                else
                {
                    textureToUse = &bodyStraightTextures[1]; // Горизонтальный
                }
            }
        }
        
        // Устанавливаем текстуру, размер и позицию спрайта
        if (textureToUse)
        {
            sprite.setTexture(*textureToUse);
            SetSpriteSize(sprite, static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE));
            sprite.setPosition(GridToPixel(segment.position));
            window.draw(sprite);
        }
    }
}

} // namespace SnakeGame

