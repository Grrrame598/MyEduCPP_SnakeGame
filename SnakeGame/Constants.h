#pragma once
#include <string>
#include <iterator>
#include <SFML/Graphics.hpp>

	const std::string RESOURCES_PATH = "Resources/";
	
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	
	const int CELL_SIZE = 40;
	
	// Константы для рекордов
	const int TOP_SCORES_DISPLAY_COUNT = 5;  // Количество топ-рекордов для отображения
	const int MAX_HIGH_SCORES = 10;          // Максимальное количество рекордов
	
	// Цвета блоков
	const sf::Color SIMPLE_BLOCK_COLOR = sf::Color(255, 0, 0);              // Красный - обычный блок (RGB: 255, 0, 0)
	const sf::Color STRONG_BLOCK_COLOR = sf::Color(255, 165, 0);           // Оранжевый - прочный блок
	const sf::Color STRONG_BLOCK_DAMAGED_COLOR = sf::Color(255, 200, 100); // Светло-оранжевый - прочный блок после удара
	const sf::Color BONUS_BLOCK_COLOR = sf::Color::Green;                  // Зеленый - блок с бонусом
	const sf::Color UNBREAKABLE_BLOCK_COLOR = sf::Color(128, 128, 128);    // Серый - неубиваемый блок
	const sf::Color TRIPLE_BLOCK_COLOR_DARK = sf::Color(138, 43, 226);     // Темно-сиреневый - тройной блок (0 ударов)
	const sf::Color TRIPLE_BLOCK_COLOR_MEDIUM = sf::Color(186, 85, 211);   // Средний сиреневый - тройной блок (1 удар)
	const sf::Color TRIPLE_BLOCK_COLOR_LIGHT = sf::Color(221, 160, 221);   // Светлый сиреневый - тройной блок (2 удара)
	
	// Коэффициенты начальной скорости мяча (угол 60 градусов)
	const float BALL_INITIAL_VELOCITY_X_COEFF = 0.5f;        // cos(60°) = 0.5
	const float BALL_INITIAL_VELOCITY_Y_COEFF = 0.8660254f;  // sin(60°) ≈ 0.8660254
	
	// Скорости объектов
	const float BALL_INITIAL_SPEED = 300.0f;    // Начальная скорость мяча
	const float PADDLE_SPEED = 450.0f;         // Скорость платформы
	
	// Позиционирование
	const float BLOCKS_START_Y = 80.0f;         // Начальная позиция блоков по Y
	const float BALL_START_OFFSET_Y = 80.0f;    // Отступ для начальной позиции мяча
	const float PADDLE_START_OFFSET_Y = 80.0f;  // Отступ для начальной позиции платформы
	const float BLOCK_OUTLINE_THICKNESS = 1.0f; // Толщина контура блока
	
	// Размеры объектов
	const float BALL_RADIUS = 20.0f;              // Радиус мяча в пикселях (диаметр 40px)
	const float PADDLE_WIDTH_MULTIPLIER = 3.0f;   // Ширина платформы (в CELL_SIZE)
	const float PADDLE_HEIGHT = 40.0f;            // Высота платформы в пикселях
	
	// Игровое поле
	const int BLOCKS_ROWS = 3;                    // Количество рядов блоков
	const int BLOCKS_COLS = 8;                    // Количество столбцов блоков
	const int BLOCK_SCORE_POINTS = 10;            // Очки за уничтожение блока
	const int STRONG_BLOCK_HITS_REQUIRED = 2;     // Количество ударов для прочного блока
	const int UNBREAKABLE_BLOCKS_PER_SIDE = 2;   // Количество неубиваемых блоков с каждой стороны во втором ряду
	
	// UI
	const float UI_OVERLAY_ALPHA = 100.0f;        // Прозрачность затемнения для попапов
	const float COUNTDOWN_FONT_SIZE = 80.0f;      // Размер шрифта для обратного отсчета
	const float COUNTDOWN_THRESHOLD_3 = 2.0f;     // Порог для отображения "3"
	const float COUNTDOWN_THRESHOLD_2 = 1.0f;     // Порог для отображения "2"
	const float COUNTDOWN_THRESHOLD_1 = 0.0f;     // Порог для отображения "1"
	const float GAME_START_DELAY = -0.2f;         // Задержка перед стартом игры после обратного отсчета
	
	// Звук
	const float BACKGROUND_MUSIC_VOLUME = 30.0f;  // Громкость фоновой музыки
	const float SOUND_EFFECTS_VOLUME = 100.0f;    // Громкость звуковых эффектов

	enum class GameState
	{
		MENU,
		WAITING,
		GAME,
		PAUSE,
		GAME_OVER,
		VICTORY,
		NAME_INPUT,
		HIGH_SCORES,
		EXIT
	};