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

using namespace SnakeGame;

GameState gameState = GameState::MENU;

int main()
{
	int seed = static_cast<int>(time(nullptr));
	srand(seed);
	
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake game!");
	window.setKeyRepeatEnabled(false);
	
	MenuState menuState;
	SoundManager soundManager(&menuState);
	UI ui(&soundManager);
	ui.initializeMainMenu();
	
	HighScoreManager highScoreManager;
	highScoreManager.loadHighScores();
	
	sf::Clock delayTimer;
	sf::Clock frameClock;
	Game game;
	
	while (window.isOpen())
	{
		if (gameState == GameState::EXIT)
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
				if (gameState == GameState::MENU)
				{
					if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
						ui.moveUp(menuState);
					else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
						ui.moveDown(menuState);
					else if (event.key.code == sf::Keyboard::Enter)
						ui.select(menuState);
					else if (event.key.code == sf::Keyboard::B || event.key.code == sf::Keyboard::Escape)
						ui.goBack(menuState);
				}
				else if (gameState == GameState::PAUSE)
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
				else if (gameState == GameState::GAME)
				{
					if (event.key.code == sf::Keyboard::P)
					{
						gameState = GameState::PAUSE;
						menuState.selectedIndex = 0;
					}
					else
						game.handleInput(event.key.code);
				}
				else if (gameState == GameState::GAME_OVER)
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
				else if (gameState == GameState::HIGH_SCORES)
				{
					if (event.key.code == sf::Keyboard::B || event.key.code == sf::Keyboard::Escape)
					{
						gameState = GameState::MENU;
						menuState.selectedIndex = 0;
					}
				}
				else if (gameState == GameState::NAME_INPUT)
				{
					if (!ui.getIsEnteringName())
					{
						if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
							ui.moveUp(menuState);
						else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
							ui.moveDown(menuState);
						else if (event.key.code == sf::Keyboard::Enter)
						{
							int selectedOption = menuState.selectedIndex;
							ui.selectNameInputOption(menuState);
							if (selectedOption == 0 && gameState == GameState::GAME_OVER)
							{
								highScoreManager.addScore("XYZ", game.getScore());
								highScoreManager.saveHighScores();
								ui.resetNameInput();
							}
						}
					}
					else
					{
						if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
						{
							char c = 'A' + (event.key.code - sf::Keyboard::A);
							ui.addCharToName(c);
						}
						else if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
						{
							char c = '0' + (event.key.code - sf::Keyboard::Num0);
							ui.addCharToName(c);
						}
						else if (event.key.code == sf::Keyboard::Backspace)
							ui.removeLastCharFromName();
						else if (event.key.code == sf::Keyboard::Enter)
						{
							highScoreManager.addScore(ui.getCurrentPlayerName(), game.getScore());
							highScoreManager.saveHighScores();
							gameState = GameState::GAME_OVER;
							menuState.selectedIndex = 0;
							ui.resetNameInput();
						}
					}
				}

			}
		}
		
		// Переходы между состояниями
		static GameState previousState = GameState::MENU;
		static GameState stateBeforeWaiting = GameState::MENU;
		
		if (gameState == GameState::WAITING && previousState != GameState::WAITING)
		{
			stateBeforeWaiting = previousState;
			delayTimer.restart();
		}
		
		if (previousState == GameState::GAME && gameState != GameState::GAME)
			soundManager.stopBackgroundMusic();
		
		if (previousState == GameState::GAME && gameState == GameState::GAME_OVER)
		{
			soundManager.playSessionEnd();
			if (highScoreManager.isHighScore(game.getScore(), Y))
				gameState = GameState::NAME_INPUT;
		}
		
		static bool previousMusicEnabled = true;
		if (!menuState.musicEnabled && previousMusicEnabled != menuState.musicEnabled)
			soundManager.stopBackgroundMusic();
		previousMusicEnabled = menuState.musicEnabled;
		
		previousState = gameState;

		// Обновление игры
		switch (gameState)
		{
			case GameState::WAITING:
			{
				float remainingSeconds = menuState.T - delayTimer.getElapsedTime().asSeconds();
				if (remainingSeconds < -0.2f) 
				{ 
					gameState = GameState::GAME;
					if (stateBeforeWaiting == GameState::MENU || stateBeforeWaiting == GameState::GAME_OVER)
						game.initialize(menuState.P);
					soundManager.playBackgroundMusic();
					frameClock.restart();
				}
				break;
			}
			case GameState::GAME:
			{
				float deltaTime = frameClock.restart().asSeconds();
				game.update(deltaTime, menuState.V, menuState.L, &soundManager);
				break;
			}
		}

		// Отрисовка
		window.clear(sf::Color::Black);
		
		if (gameState == GameState::MENU)
		{
			ui.draw(window, menuState);
			ui.drawControlsHint(window);
		}
		else if (gameState == GameState::WAITING)
		{
			float remainingSeconds = menuState.T - delayTimer.getElapsedTime().asSeconds();
			ui.drawCountdown(window, remainingSeconds);
		}
		else if (gameState == GameState::PAUSE)
			ui.drawPauseMenu(window, menuState);
		else if (gameState == GameState::GAME)
		{
			game.draw(window);
			sf::Text scoreText;
			scoreText.setFont(ui.getFont());
			scoreText.setString("Score: " + std::to_string(game.getScore()));
			scoreText.setCharacterSize(24u);
			scoreText.setPosition(10, 10);
			scoreText.setFillColor(sf::Color::White);
			window.draw(scoreText);
		}
		else if (gameState == GameState::GAME_OVER)
			ui.drawGameOverMenu(window, menuState, game.getScore(), highScoreManager);
		else if (gameState == GameState::NAME_INPUT)
			ui.drawNameInputPopup(window, menuState);
		else if (gameState == GameState::HIGH_SCORES)
		{
			highScoreManager.drawHighScoresTable(window, ui.getFont());
			ui.drawControlsHint(window);
		}
		
		window.display();
	}

	return 0;
}
