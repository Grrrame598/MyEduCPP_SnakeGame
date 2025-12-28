#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

namespace SnakeGame
{

class SoundManager;
class HighScoreManager;

const std::string MENU_START_GAME = "Start Game";
const std::string MENU_DIFFICULTY = "Difficulty";
const std::string MENU_HIGH_SCORES = "High Scores";
const std::string MENU_SETTINGS = "Settings";
const std::string MENU_EXIT = "Exit";

const std::string DIFFICULTY_EASY = "Easy";
const std::string DIFFICULTY_EASIER_THAN_MEDIUM = "Easier Than Medium";
const std::string DIFFICULTY_MEDIUM = "Medium";
const std::string DIFFICULTY_EASIER_THAN_HARD = "Easier Than Hard";
const std::string DIFFICULTY_HARD = "Hard";

const std::string SETTINGS_SOUND = "Sound";
const std::string SETTINGS_MUSIC = "Music";

const std::string PAUSE_CONTINUE = "Continue";
const std::string PAUSE_EXIT_TO_MENU = "Exit to Menu";

const std::string GAME_OVER_START_GAME = "Start game";
const std::string GAME_OVER_TO_MENU = "To main menu";

const std::string VICTORY_START_GAME = "Start game";
const std::string VICTORY_TO_MENU = "To main menu";

const std::string NAME_INPUT_TITLE = "Enter name";
const std::string NAME_INPUT_NO = "No";
const std::string NAME_INPUT_YES = "Yes";

const std::string CHECKBOX_ON = "[x]";
const std::string CHECKBOX_OFF = "[_]";

struct MenuItem
{
	std::string text;
};

class MenuState
{
private:
	bool soundEnabled = true;
	bool musicEnabled = true;
	int selectedIndex = 0;
	
	int V = 1;
	int P = 2;
	int L = 1;
	float T = 3.0f;
	std::string difficultyLevel = DIFFICULTY_EASY;

public:
	MenuState() = default;
	
	// Getters
	bool getSoundEnabled() const { return soundEnabled; }
	bool getMusicEnabled() const { return musicEnabled; }
	int getSelectedIndex() const { return selectedIndex; }
	int getV() const { return V; }
	int getP() const { return P; }
	int getL() const { return L; }
	float getT() const { return T; }
	const std::string& getDifficultyLevel() const { return difficultyLevel; }
	
	// Setters
	void setSoundEnabled(bool value) { soundEnabled = value; }
	void setMusicEnabled(bool value) { musicEnabled = value; }
	void setSelectedIndex(int value) { selectedIndex = value; }
	void setV(int value) { V = value; }
	void setP(int value) { P = value; }
	void setL(int value) { L = value; }
	void setT(float value) { T = value; }
	void setDifficultyLevel(const std::string& value) { difficultyLevel = value; }
};

class UI
{
public:
	UI(SoundManager* soundManager);
	void initializeMainMenu();
	void openDifficultyMenu();
	void openSettingsMenu();
	void goBack(MenuState& state);
	void draw(sf::RenderWindow& window, const MenuState& state);
	
	void drawPauseMenu(sf::RenderWindow& window, const MenuState& state);
	void drawGameOverMenu(sf::RenderWindow& window, const MenuState& state, int score, const HighScoreManager& highScoreManager);
	void drawVictoryMenu(sf::RenderWindow& window, const MenuState& state);
	void selectPopupMenu(MenuState& state, const std::string menuItems[], int itemCount);
	
	void drawCountdown(sf::RenderWindow& window, float remainingSeconds);
	
	void drawNameInputPopup(sf::RenderWindow& window, MenuState& state);
	void addCharToName(char c);
	void removeLastCharFromName();
	void selectNameInputOption(MenuState& state);
	std::string getCurrentPlayerName() const { return currentPlayerName; }
	bool getIsEnteringName() const { return isEnteringName; }
	void resetNameInput();

	void moveUp(MenuState& state);
	void moveDown(MenuState& state);
	void select(MenuState& state);
	
	const sf::Font& getFont() const { return font; }
	void drawControlsHint(sf::RenderWindow& window) const;
	
private:
	std::vector<std::vector<MenuItem>> menuStack;
	const std::string pauseMenuItems[2] = {PAUSE_CONTINUE, PAUSE_EXIT_TO_MENU};
	const std::string gameOverMenuItems[2] = {GAME_OVER_START_GAME, GAME_OVER_TO_MENU};
	const std::string victoryMenuItems[2] = {VICTORY_START_GAME, VICTORY_TO_MENU};
	sf::Font font;
	SoundManager* soundManager;
	std::string currentPlayerName = "XYZ";
	bool isEnteringName = false;
};

} // namespace SnakeGame
