#pragma once
#include <string>
#include <iterator>

	const std::string RESOURCES_PATH = "Resources/";
	
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	
	const int CELL_SIZE = 40;
	const int GRID_WIDTH = SCREEN_WIDTH / CELL_SIZE;
	const int GRID_HEIGHT = SCREEN_HEIGHT / CELL_SIZE;
	
	const float MOVE_INTERVAL = 0.5f;
	
	const int X = 5;
	const int Y = 10;
	
	enum class Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	enum class GameState
	{
		MENU,
		WAITING,
		GAME,
		PAUSE,
		GAME_OVER,
		NAME_INPUT,
		HIGH_SCORES,
		EXIT
	};
	
	extern GameState gameState;