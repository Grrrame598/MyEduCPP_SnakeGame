#include "UI.h"
#include "Constants.h"
#include "SoundManager.h"
#include "HighScoreManager.h"
#include "GameStateManager.h"
#include <cassert>

using namespace SnakeGame;

UI::UI(SoundManager* soundManager)
{
    this->soundManager = soundManager;
}

void UI::update(float deltaTime, const sf::RenderWindow* window)
{
    // Menu doesn't require continuous updates
}

void UI::draw(sf::RenderWindow& window)
{
    drawMenu(window, menuState);
}

void UI::handleInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::W || key == sf::Keyboard::Up)
        moveUp(menuState);
    else if (key == sf::Keyboard::S || key == sf::Keyboard::Down)
        moveDown(menuState);
    else if (key == sf::Keyboard::Enter)
        select(menuState);
    else if (key == sf::Keyboard::B || key == sf::Keyboard::Escape)
        goBack(menuState);
}

void UI::initializeMainMenu()
{
    assert(font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"));

	std::vector<MenuItem> mainMenu;
	MenuItem item1;
    item1.text = MENU_START_GAME;
	mainMenu.push_back(item1);
	
	MenuItem item2;
    item2.text = MENU_DIFFICULTY;
	mainMenu.push_back(item2);
	
	MenuItem item3;
    item3.text = MENU_HIGH_SCORES;
	mainMenu.push_back(item3);
	
	MenuItem item4;
    item4.text = MENU_SETTINGS;
	mainMenu.push_back(item4);
	
	MenuItem item5;
    item5.text = MENU_EXIT;
	mainMenu.push_back(item5);
	
	menuStack.push_back(mainMenu);
}
	
void UI::openDifficultyMenu()
{
	std::vector<MenuItem> difficultyMenu;
	
	MenuItem diff1;
    diff1.text = DIFFICULTY_EASY;
	difficultyMenu.push_back(diff1);
	
	MenuItem diff2;
    diff2.text = DIFFICULTY_EASIER_THAN_MEDIUM;
	difficultyMenu.push_back(diff2);
	
	MenuItem diff3;
    diff3.text = DIFFICULTY_MEDIUM;
	difficultyMenu.push_back(diff3);
	
	MenuItem diff4;
    diff4.text = DIFFICULTY_EASIER_THAN_HARD;
	difficultyMenu.push_back(diff4);
	
	MenuItem diff5;
    diff5.text = DIFFICULTY_HARD;
	difficultyMenu.push_back(diff5);
	
	menuStack.push_back(difficultyMenu);
}

void UI::openSettingsMenu()
{
	std::vector<MenuItem> settingsMenu;
	
	MenuItem set1;
    set1.text = SETTINGS_SOUND;
	settingsMenu.push_back(set1);
	
	MenuItem set2;
    set2.text = SETTINGS_MUSIC;
	settingsMenu.push_back(set2);
	
	menuStack.push_back(settingsMenu);
}

void UI::goBack(MenuState& state)
{
    if (menuStack.size() > 1)
    {
        if (soundManager)
            soundManager->playEnter();
        menuStack.pop_back();
        state.setSelectedIndex(0);
    }
}

void UI::drawMenu(sf::RenderWindow& window, const MenuState& state)
{
    const auto& currentMenu = menuStack.back();
    
    const float fontSize = 30.0f;
    const float deltaY = 50.0f;
    const float menuStartY = 210.0f;
    const float centerX = SCREEN_WIDTH / 2.0f;
    
    for (size_t i = 0; i < currentMenu.size(); i++)
    {
        sf::Text text;
        text.setFont(font);
        
        std::string displayText = currentMenu[i].text;
        
        if (currentMenu[i].text == SETTINGS_SOUND)
        {
            displayText = SETTINGS_SOUND + " " + (state.getSoundEnabled() ? CHECKBOX_ON : CHECKBOX_OFF);
        }
        else if (currentMenu[i].text == SETTINGS_MUSIC)
        {
            displayText = SETTINGS_MUSIC + " " + (state.getMusicEnabled() ? CHECKBOX_ON : CHECKBOX_OFF);
        }
        
        text.setString(displayText);
        text.setCharacterSize(static_cast<unsigned int>(fontSize));
        
        float yPos = menuStartY + (i * deltaY);
        text.setPosition(centerX, yPos);
        text.setOrigin(text.getLocalBounds().width / 2, 0);
        
        text.setFillColor(i == state.getSelectedIndex() ? sf::Color::Green : sf::Color::White);
        window.draw(text);
    }
}

void UI::drawPauseMenu(sf::RenderWindow& window, const MenuState& state)
{
    sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(overlay);
    
    const float fontSize = 30.0f;
    const float deltaY = 50.0f;
    const float menuStartY = 250.0f;
    const float centerX = SCREEN_WIDTH / 2.0f;
    
    for (size_t i = 0; i < 2; i++)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(pauseMenuItems[i]);
        text.setCharacterSize(static_cast<unsigned int>(fontSize));
        
        float yPos = menuStartY + (i * deltaY);
        text.setPosition(centerX, yPos);
        text.setOrigin(text.getLocalBounds().width / 2, 0);
        text.setFillColor(i == state.getSelectedIndex() ? sf::Color::Green : sf::Color::White);
        
        window.draw(text);
    }
}

void UI::drawGameOverMenu(sf::RenderWindow& window, const MenuState& state, int score, const HighScoreManager& highScoreManager)
{
    sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(overlay);
    
    const float scoreFontSize = 36.0f;
    const float recordsFontSize = 20.0f;
    const float menuFontSize = 30.0f;
    const float deltaY = 50.0f;
    const float recordsDeltaY = 22.0f;
    const float scoreY = 100.0f;
    const float recordsY = 160.0f;
    const float menuStartY = 300.0f;
    const float centerX = SCREEN_WIDTH / 2.0f;
    const float recordsStartX = 350.0f;
    
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(static_cast<unsigned int>(scoreFontSize));
    scoreText.setPosition(centerX, scoreY);
    scoreText.setOrigin(scoreText.getLocalBounds().width / 2, 0);
    scoreText.setFillColor(sf::Color::White);
    window.draw(scoreText);

    std::vector<HighScoreEntry> topScores = highScoreManager.getTopScores(X);
    size_t index = 1;
    for (const auto& entry : topScores)
    {
        sf::Text recordText;
        recordText.setFont(font);
        recordText.setString(std::to_string(index) + ". " + entry.getPlayerName() + "....." + std::to_string(entry.getScore()));
        recordText.setCharacterSize(static_cast<unsigned int>(recordsFontSize));
        
        float yPos = recordsY + ((index - 1) * recordsDeltaY);
        recordText.setPosition(recordsStartX, yPos);
        recordText.setOrigin(0, 0);
        recordText.setFillColor(sf::Color::White);
        window.draw(recordText);
        index++;
    }

    for (size_t i = 0; i < 2; i++)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(gameOverMenuItems[i]);
        text.setCharacterSize(static_cast<unsigned int>(menuFontSize));
        
        float yPos = menuStartY + (i * deltaY);
        text.setPosition(centerX, yPos);
        text.setOrigin(text.getLocalBounds().width / 2, 0);
        text.setFillColor(i == state.getSelectedIndex() ? sf::Color::Green : sf::Color::White);
        window.draw(text);
    }
}

void UI::drawVictoryMenu(sf::RenderWindow& window, const MenuState& state)
{
    sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(overlay);
    
    const float titleFontSize = 48.0f;
    const float menuFontSize = 30.0f;
    const float deltaY = 50.0f;
    const float titleY = 200.0f;
    const float menuStartY = 300.0f;
    const float centerX = SCREEN_WIDTH / 2.0f;
    
    // Заголовок "Victory!"
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Victory!");
    titleText.setCharacterSize(static_cast<unsigned int>(titleFontSize));
    titleText.setPosition(centerX, titleY);
    titleText.setOrigin(titleText.getLocalBounds().width / 2, 0);
    titleText.setFillColor(sf::Color::Yellow);
    window.draw(titleText);

    // Меню выбора
    for (size_t i = 0; i < 2; i++)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(victoryMenuItems[i]);
        text.setCharacterSize(static_cast<unsigned int>(menuFontSize));
        
        float yPos = menuStartY + (i * deltaY);
        text.setPosition(centerX, yPos);
        text.setOrigin(text.getLocalBounds().width / 2, 0);
        text.setFillColor(i == state.getSelectedIndex() ? sf::Color::Green : sf::Color::White);
        window.draw(text);
    }
}

void UI::drawCountdown(sf::RenderWindow& window, float remainingSeconds)
{
    sf::Text text;
    text.setFont(font);
    
    if (remainingSeconds > 2.0f)
        text.setString("3");
    else if (remainingSeconds > 1.0f)
        text.setString("2");
    else if (remainingSeconds > 0.0f)
        text.setString("1");
    else
        text.setString("GO!");
    
    text.setCharacterSize(80u);
    text.setFillColor(sf::Color::White);
    text.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    window.draw(text);
}

void UI::moveDown(MenuState& state)
{
    const auto& currentMenu = menuStack.back();
    if (state.getSelectedIndex() < currentMenu.size() - 1)
    {
        state.setSelectedIndex(state.getSelectedIndex() + 1);
        if (soundManager)
            soundManager->playMenuHover();
    }
}

void UI::moveUp(MenuState& state)
{
    if (state.getSelectedIndex() > 0)
    {
        state.setSelectedIndex(state.getSelectedIndex() - 1);
        if (soundManager)
            soundManager->playMenuHover();
    }
}

void UI::selectPopupMenu(MenuState& state, const std::string menuItems[], int itemCount)
{
    if (soundManager)
        soundManager->playEnter();
    
    std::string selectedText = menuItems[state.getSelectedIndex()];
    
    if (selectedText == PAUSE_CONTINUE || selectedText == GAME_OVER_START_GAME || selectedText == VICTORY_START_GAME)
        gameStateManager.setState(GameState::WAITING);
    else if (selectedText == PAUSE_EXIT_TO_MENU || selectedText == GAME_OVER_TO_MENU || selectedText == VICTORY_TO_MENU)
    {
        gameStateManager.setState(GameState::MENU);
        state.setSelectedIndex(0);
    }
}

void UI::addCharToName(char c)
{
    if (((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) && currentPlayerName.length() < 15)
        currentPlayerName += c;
}

void UI::removeLastCharFromName()
{
    if (!currentPlayerName.empty())
        currentPlayerName.pop_back();
}

void UI::resetNameInput()
{
    currentPlayerName = "XYZ";
    isEnteringName = false;
}

void UI::drawNameInputPopup(sf::RenderWindow& window, MenuState& state)
{
    sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(overlay);
    
    const float titleFontSize = 40.0f;
    const float menuFontSize = 30.0f;
    const float nameInputFontSize = 28.0f;
    const float deltaY = 60.0f;
    const float centerX = SCREEN_WIDTH / 2.0f;
    const float popupStartY = SCREEN_HEIGHT / 2.0f - 100.0f;
    
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString(NAME_INPUT_TITLE);
    titleText.setCharacterSize(static_cast<unsigned int>(titleFontSize));
    titleText.setPosition(centerX, popupStartY);
    titleText.setOrigin(titleText.getLocalBounds().width / 2, 0);
    titleText.setFillColor(sf::Color::White);
    window.draw(titleText);
    
    const std::string nameInputMenuItems[2] = {NAME_INPUT_NO, NAME_INPUT_YES};
    for (size_t i = 0; i < 2; i++)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(nameInputMenuItems[i]);
        text.setCharacterSize(static_cast<unsigned int>(menuFontSize));
        
        float yPos = popupStartY + deltaY + (i * deltaY);
        text.setPosition(centerX, yPos);
        text.setOrigin(text.getLocalBounds().width / 2, 0);
        text.setFillColor(i == state.getSelectedIndex() ? sf::Color::Green : sf::Color::White);
        window.draw(text);
    }
    
    if (isEnteringName)
    {
        sf::Text nameLabelText;
        nameLabelText.setFont(font);
        nameLabelText.setString("Name: ");
        nameLabelText.setCharacterSize(static_cast<unsigned int>(nameInputFontSize));
        float nameInputY = SCREEN_HEIGHT - 100.0f;
        nameLabelText.setPosition(centerX, nameInputY);
        nameLabelText.setOrigin(nameLabelText.getLocalBounds().width / 2, 0);
        nameLabelText.setFillColor(sf::Color::White);
        window.draw(nameLabelText);
        
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(currentPlayerName);
        nameText.setCharacterSize(static_cast<unsigned int>(nameInputFontSize));
        float nameTextY = nameInputY + nameLabelText.getLocalBounds().height + 10.0f;
        nameText.setPosition(centerX, nameTextY);
        nameText.setOrigin(nameText.getLocalBounds().width / 2, 0);
        nameText.setFillColor(sf::Color::Yellow);
        window.draw(nameText);
    }
}

void UI::selectNameInputOption(MenuState& state)
{
    if (soundManager)
        soundManager->playEnter();
    
    if (state.getSelectedIndex() == 0)
    {
        // Возвращаемся в предыдущее состояние (GAME_OVER или VICTORY)
        // Состояние уже будет восстановлено в GameMain.cpp после сохранения имени
        state.setSelectedIndex(0);
    }
    else if (state.getSelectedIndex() == 1)
    {
        isEnteringName = true;
        state.setSelectedIndex(0);
    }
}

void UI::select(MenuState& state)
{
    if (soundManager)
        soundManager->playEnter();
    
    const auto& currentMenu = menuStack.back();
    std::string selectedText = currentMenu[state.getSelectedIndex()].text;
    
    if (menuStack.size() == 1)
    {
        if (selectedText == MENU_START_GAME)
            gameStateManager.setState(GameState::WAITING);
        else if (selectedText == MENU_DIFFICULTY)
        {
            openDifficultyMenu();
            state.setSelectedIndex(0);
        }
        else if (selectedText == MENU_HIGH_SCORES)
            gameStateManager.setState(GameState::HIGH_SCORES);
        else if (selectedText == MENU_SETTINGS)
        {
            openSettingsMenu();
            state.setSelectedIndex(0);
        }
        else if (selectedText == MENU_EXIT)
            gameStateManager.setState(GameState::EXIT);
    }
    else if (menuStack.size() == 2)
    {
        std::string firstItemText = currentMenu[0].text;
        
        if (firstItemText == DIFFICULTY_EASY || firstItemText == DIFFICULTY_MEDIUM)
        {
            if (selectedText == DIFFICULTY_EASY)
            {
                state.setDifficultyLevel(DIFFICULTY_EASY);
                state.setV(1); state.setP(2); state.setL(1);
            }
            else if (selectedText == DIFFICULTY_EASIER_THAN_MEDIUM)
            {
                state.setDifficultyLevel(DIFFICULTY_EASIER_THAN_MEDIUM);
                state.setV(2); state.setP(4); state.setL(1);
            }
            else if (selectedText == DIFFICULTY_MEDIUM)
            {
                state.setDifficultyLevel(DIFFICULTY_MEDIUM);
                state.setV(3); state.setP(6); state.setL(2);
            }
            else if (selectedText == DIFFICULTY_EASIER_THAN_HARD)
            {
                state.setDifficultyLevel(DIFFICULTY_EASIER_THAN_HARD);
                state.setV(4); state.setP(8); state.setL(2);
            }
            else if (selectedText == DIFFICULTY_HARD)
            {
                state.setDifficultyLevel(DIFFICULTY_HARD);
                state.setV(5); state.setP(10); state.setL(3);
            }
            
            if (firstItemText == DIFFICULTY_EASY || firstItemText == DIFFICULTY_MEDIUM)
            {
                menuStack.pop_back();
                state.setSelectedIndex(1);
            }
        }
        else if (firstItemText == SETTINGS_SOUND || firstItemText == SETTINGS_MUSIC)
        {
            if (selectedText == SETTINGS_SOUND)
                state.setSoundEnabled(!state.getSoundEnabled());
            else if (selectedText == SETTINGS_MUSIC)
                state.setMusicEnabled(!state.getMusicEnabled());
        }
    }
}

void UI::drawControlsHint(sf::RenderWindow& window) const
{
    const float hintFontSize = 18.0f;
    const float hintY = SCREEN_HEIGHT - 40.0f;
    const float hintStartX = 120.0f;
    
    std::string hintText = "WADS/Arrows: Navigate __ Enter: Select __ B/Escape: Back __ P: Pause";
    
    sf::Text hint;
    hint.setFont(font);
    hint.setString(hintText);
    hint.setCharacterSize(static_cast<unsigned int>(hintFontSize));
    hint.setPosition(hintStartX, hintY);
    hint.setOrigin(0, 0);
    hint.setFillColor(sf::Color(200, 200, 200));
    window.draw(hint);
}