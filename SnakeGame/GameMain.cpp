#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>  
#include <cstdlib>
#include <ctime>
#include "Constants.h"
#include "UI.h"
#include "Game.h"
#include "SoundManager.h"
#include "HighScoreManager.h"
#include "GameStateManager.h"

namespace ArkanoidGame {
    GameStateManager gameStateManager;
}

using namespace ArkanoidGame;

int main()
{
	int seed = static_cast<int>(time(nullptr));
	srand(seed);
	
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Arkanoid");
	window.setKeyRepeatEnabled(false);
	
	UI ui(nullptr);
	ui.initializeMainMenu();
	MenuState& menuState = ui.getMenuState();
	SoundManager soundManager(&menuState);
	ui.setSoundManager(&soundManager);
	
	HighScoreManager highScoreManager;
	highScoreManager.loadHighScores();
	
	sf::Clock delayTimer;
	sf::Clock frameClock;
	Game game;
	
	static GameState previousState = GameState::MENU;
	static GameState stateBeforeWaiting = GameState::MENU;
	
	while (window.isOpen())
	{
		if (gameStateManager.getCurrentState() == GameState::EXIT)
		{
			window.close();
			break;
		}
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			
			if (event.type == sf::Event::KeyPressed)
			{
				if (gameStateManager.getCurrentState() == GameState::MENU)
				{
					ui.handleInput(event.key.code);
				}
				else if (gameStateManager.getCurrentState() == GameState::PAUSE)
				{
					if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
						ui.moveUp(menuState);
					else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
						ui.moveDown(menuState);
					else if (event.key.code == sf::Keyboard::Enter)
					{
						const std::string pauseMenuItems[2] = {PAUSE_CONTINUE, PAUSE_EXIT_TO_MENU};
						ui.selectPopupMenu(menuState, pauseMenuItems, 2);
					}
				}
				else if (gameStateManager.getCurrentState() == GameState::GAME)
				{
					if (event.key.code == sf::Keyboard::P)
					{
						gameStateManager.setState(GameState::PAUSE);
						menuState.setSelectedIndex(0);
						ui.clearMenuStack(); // Очищаем вектор для попап-меню
					}
					else
						game.handleInput(event.key.code);
				}
				else if (gameStateManager.getCurrentState() == GameState::GAME_OVER)
				{
					if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
						ui.moveUp(menuState);
					else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
						ui.moveDown(menuState);
					else if (event.key.code == sf::Keyboard::Enter)
					{
						const std::string gameOverMenuItems[2] = {GAME_OVER_START_GAME, GAME_OVER_TO_MENU};
						ui.selectPopupMenu(menuState, gameOverMenuItems, 2);
					}
				}
				else if (gameStateManager.getCurrentState() == GameState::VICTORY)
				{
					if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
						ui.moveUp(menuState);
					else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
						ui.moveDown(menuState);
					else if (event.key.code == sf::Keyboard::Enter)
					{
						const std::string victoryMenuItems[2] = {VICTORY_START_GAME, VICTORY_TO_MENU};
						ui.selectPopupMenu(menuState, victoryMenuItems, 2);
					}
				}
				else if (gameStateManager.getCurrentState() == GameState::HIGH_SCORES)
				{
					if (event.key.code == sf::Keyboard::B || event.key.code == sf::Keyboard::Escape)
					{
						gameStateManager.setState(GameState::MENU);
						menuState.setSelectedIndex(0);
						ui.initializeMainMenu(); // Восстанавливаем вектор при выходе из HIGH_SCORES
					}
				}
			}
		}
		if (gameStateManager.getCurrentState() == GameState::WAITING && previousState != GameState::WAITING)
		{
			stateBeforeWaiting = previousState;
			delayTimer.restart();
		}
		
		if (previousState == GameState::GAME && gameStateManager.getCurrentState() != GameState::GAME)
			soundManager.stopBackgroundMusic();
		
		// Проверка условий окончания игры
		if (gameStateManager.getCurrentState() == GameState::GAME)
		{
			if (game.isVictory())
			{
				gameStateManager.setState(GameState::VICTORY);
				menuState.setSelectedIndex(0);
				ui.clearMenuStack(); // Очищаем вектор для попап-меню
			}
			else if (game.isBallFell())
			{
				game.loseLife(); // Уменьшаем жизни и загружаем сохранение
				
				if (game.isGameOver()) // Проверяем, закончилась ли игра (lives == 0)
				{
					gameStateManager.setState(GameState::GAME_OVER);
					menuState.setSelectedIndex(0);
					ui.clearMenuStack(); // Очищаем вектор для попап-меню
				}
				// Если lives > 0, игра продолжается с восстановленным состоянием
			}
		}
		if (previousState == GameState::GAME && gameStateManager.getCurrentState() == GameState::GAME_OVER)
		{
			soundManager.playSessionEnd();
		}
		
		if (previousState == GameState::GAME && gameStateManager.getCurrentState() == GameState::VICTORY)
		{
			soundManager.playSessionEnd();
		}
		
		static bool previousMusicEnabled = true;
		if (menuState.getMusicEnabled() != previousMusicEnabled)
		{
			if (!menuState.getMusicEnabled())
				soundManager.stopBackgroundMusic();
			else if (gameStateManager.getCurrentState() == GameState::GAME)
				soundManager.playBackgroundMusic();
		}
		previousMusicEnabled = menuState.getMusicEnabled();
		
		previousState = gameStateManager.getCurrentState();

		switch (gameStateManager.getCurrentState())
		{
		case GameState::WAITING:
		{
			// Ожидание с обратным отсчетом перед стартом игры
			float remainingSeconds = menuState.getT() - delayTimer.getElapsedTime().asSeconds();
			if (remainingSeconds < GAME_START_DELAY) 
			{ 
				gameStateManager.setState(GameState::GAME);
				// Перезапуск игры при старте из меню/геймовер/виктори
				if (stateBeforeWaiting == GameState::MENU || stateBeforeWaiting == GameState::GAME_OVER || stateBeforeWaiting == GameState::VICTORY)
				{
					game.initialize(&soundManager);
					game.restartGame();
					soundManager.playBackgroundMusic();
					frameClock.restart();
				}
			}
			break;
		}
		case GameState::GAME:
			{
				float deltaTime = frameClock.restart().asSeconds();
				game.update(deltaTime, &window);
				break;
			}
		}

		window.clear(sf::Color::Black);
		
		if (gameStateManager.getCurrentState() == GameState::MENU)
		{
			ui.draw(window);
			ui.drawControlsHint(window);
		}
		else if (gameStateManager.getCurrentState() == GameState::WAITING)
		{
			float remainingSeconds = menuState.getT() - delayTimer.getElapsedTime().asSeconds();
			ui.drawCountdown(window, remainingSeconds);
		}
		else if (gameStateManager.getCurrentState() == GameState::PAUSE)
		{
			game.draw(window);
			sf::Text scoreText;
			scoreText.setFont(ui.getFont());
			scoreText.setString("Score: " + std::to_string(game.getScore()));
			scoreText.setCharacterSize(static_cast<unsigned int>(UI_GAME_INFO_FONT_SIZE));
			scoreText.setPosition(UI_GAME_INFO_X, UI_GAME_INFO_SCORE_Y);
			scoreText.setFillColor(sf::Color::White);
			window.draw(scoreText);
			
			sf::Text livesText;
			livesText.setFont(ui.getFont());
			livesText.setString("Lives: " + std::to_string(game.getLives()));
			livesText.setCharacterSize(static_cast<unsigned int>(UI_GAME_INFO_FONT_SIZE));
			livesText.setPosition(UI_GAME_INFO_X, UI_GAME_INFO_LIVES_Y);
			livesText.setFillColor(sf::Color::White);
			window.draw(livesText);
			
			ui.drawPauseMenu(window, menuState);
		}
		else if (gameStateManager.getCurrentState() == GameState::GAME)
		{
			game.draw(window);
			sf::Text scoreText;
			scoreText.setFont(ui.getFont());
			scoreText.setString("Score: " + std::to_string(game.getScore()));
			scoreText.setCharacterSize(static_cast<unsigned int>(UI_GAME_INFO_FONT_SIZE));
			scoreText.setPosition(UI_GAME_INFO_X, UI_GAME_INFO_SCORE_Y);
			scoreText.setFillColor(sf::Color::White);
			window.draw(scoreText);
			
			sf::Text livesText;
			livesText.setFont(ui.getFont());
			livesText.setString("Lives: " + std::to_string(game.getLives()));
			livesText.setCharacterSize(static_cast<unsigned int>(UI_GAME_INFO_FONT_SIZE));
			livesText.setPosition(UI_GAME_INFO_X, UI_GAME_INFO_LIVES_Y);
			livesText.setFillColor(sf::Color::White);
			window.draw(livesText);
		}
		else if (gameStateManager.getCurrentState() == GameState::GAME_OVER)
		{
			game.draw(window);
			ui.drawGameOverMenu(window, menuState, game.getScore(), highScoreManager);
		}
		else if (gameStateManager.getCurrentState() == GameState::VICTORY)
		{
			game.draw(window);
			ui.drawVictoryMenu(window, menuState, game.getScore(), highScoreManager);
		}
		else if (gameStateManager.getCurrentState() == GameState::HIGH_SCORES)
		{
			highScoreManager.drawHighScoresTable(window, ui.getFont());
			ui.drawControlsHint(window);
		}
		
		window.display();
	}

	return 0;
}
