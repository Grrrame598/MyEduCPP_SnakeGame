# План работы над заданием "Используем ООП в Арканоиде"

## Пункт 1: Перевести структуры на классы с инкапсуляцией

### 1.1. MenuState struct → class
- Приватные поля: soundEnabled, musicEnabled, selectedIndex, V, P, L, T, difficultyLevel
- Публичные getters/setters для всех полей
- Конструктор по умолчанию

### 1.2. HighScoreEntry struct → class
- Приватные поля: playerName, score
- Публичные getters/setters
- Конструктор с параметрами

### 1.3. MenuItem struct (опционально)
- Одно поле text - решить, нужно ли переводить в класс

## Пункт 2: GameState с RAII

### Создать класс GameStateManager
- Приватное поле: GameState currentState
- Публичные методы: getCurrentState(), setState()
- Конструктор инициализирует состояние
- Деструктор для RAII (автоматическая очистка)

## Пункт 3: Класс Paddle (платформа)

### Создать новый класс Paddle
- Приватные поля: shape (sf::RectangleShape), position, speed, width, height
- Публичные методы: update(), draw(), handleKeyboardInput(), handleMouseInput()
- Конструктор инициализирует платформу
- Платформа внизу экрана, движение влево/вправо

## Пункт 4: Класс Ball (шарик)

### Создать новый класс Ball
- Приватные поля: shape (sf::CircleShape), position, velocity, radius, speed
- Публичные методы: update(), draw(), bounceHorizontal(), bounceVertical()
- Методы для проверки столкновений с платформой и стенами
- Конструктор инициализирует шарик

## План коммитов

Один коммит: "Перевод структур в классы, создание Paddle и Ball, реализация RAII для GameState"

