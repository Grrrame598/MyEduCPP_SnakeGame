#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Constants.h"

namespace SnakeGame
{

struct HighScoreEntry
{
    std::string playerName;
    int score;
};

class HighScoreManager
{
public:
    void loadHighScores();
    void saveHighScores();
    void addScore(const std::string& name, int score);
    std::vector<HighScoreEntry> getTopScores(int count = Y) const;
    bool isHighScore(int score, int topCount = Y) const;
    size_t getScoreCount() const { return scores.size(); }
    void drawHighScoresTable(sf::RenderWindow& window, const sf::Font& font) const;

private:
    std::vector<HighScoreEntry> scores;
    const std::string HIGHSCORES_FILE = RESOURCES_PATH + "save/highscores.txt";
    const size_t MAX_SCORES = Y;
    void sortScores();
};

} // namespace SnakeGame