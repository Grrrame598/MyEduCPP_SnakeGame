#include "HighScoreManager.h"
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <sstream>   // Для std::istringstream
#include <direct.h>  // Для _mkdir (Windows)

namespace ArkanoidGame
{

void HighScoreManager::saveHighScores()
{
    std::string directoryPath = RESOURCES_PATH + "save";
    _mkdir(directoryPath.c_str());
    
    std::ofstream file(HIGHSCORES_FILE);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file for writing: " << HIGHSCORES_FILE << std::endl;
        return;
    }
    
    for (const auto& entry : scores)
        file << entry.getPlayerName() << " " << entry.getScore() << std::endl;
    
    file.close();
}

void HighScoreManager::sortScores()
{
    std::sort(scores.begin(), scores.end(), 
        [](const HighScoreEntry& a, const HighScoreEntry& b) 
        {
            if (a.getScore() != b.getScore())
                return a.getScore() > b.getScore();
            return a.getPlayerName() < b.getPlayerName();
        });
}

void HighScoreManager::addScore(const std::string& name, int score)
{
    if (scores.size() < MAX_SCORES)
    {
        HighScoreEntry entry(name, score);
        scores.push_back(entry);
        sortScores();
        return;
    }
    
    sortScores();
    
    if (score > scores.back().getScore())
    {
        HighScoreEntry entry(name, score);
        scores.push_back(entry);
        sortScores();
        
        if (scores.size() > MAX_SCORES)
            scores.resize(MAX_SCORES);
    }
}

bool HighScoreManager::isHighScore(int score, int topCount) const
{
    if (scores.size() < static_cast<size_t>(topCount))
        return true;
    
    std::vector<HighScoreEntry> sortedScores = scores;
    std::sort(sortedScores.begin(), sortedScores.end(),
        [](const HighScoreEntry& a, const HighScoreEntry& b)
        {
            if (a.getScore() != b.getScore())
                return a.getScore() > b.getScore();
            return a.getPlayerName() < b.getPlayerName();
        });
    
    return score > sortedScores[topCount - 1].getScore();
}

std::vector<HighScoreEntry> HighScoreManager::getTopScores(int count) const
{
    std::vector<HighScoreEntry> sortedScores = scores;
    std::sort(sortedScores.begin(), sortedScores.end(),
        [](const HighScoreEntry& a, const HighScoreEntry& b)
        {
            if (a.getScore() != b.getScore())
                return a.getScore() > b.getScore();
            return a.getPlayerName() < b.getPlayerName();
        });
    
    size_t returnCount = std::min(sortedScores.size(), static_cast<size_t>(count));
    
    if (returnCount == sortedScores.size())
        return sortedScores;
    else
        return std::vector<HighScoreEntry>(sortedScores.begin(), sortedScores.begin() + returnCount);
}

void HighScoreManager::loadHighScores()
{
    scores.clear();
    
    std::ifstream file(HIGHSCORES_FILE);
    if (!file.is_open())
        return;
    
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        
        std::istringstream iss(line);
        std::vector<std::string> parts;
        std::string part;
        while (iss >> part)
            parts.push_back(part);
        
        if (parts.size() < 2)
            continue;
        
        try
        {
            int score = std::stoi(parts.back());
            
            std::string name = parts[0];
            for (size_t i = 1; i < parts.size() - 1; ++i)
                name += " " + parts[i];
            
            HighScoreEntry entry(name, score);
            scores.push_back(entry);
        }
        catch (const std::exception&)
        {
            continue;
        }
    }
    
    file.close();
    sortScores();
}

void HighScoreManager::drawHighScoresTable(sf::RenderWindow& window, const sf::Font& font) const
{
    sf::RectangleShape overlay(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);
    
    const float fontSize = 24.0f;
    const float deltaY = 35.0f;
    const float tableStartY = 100.0f;
    const float tableStartX = 300.0f;
    
    size_t index = 1;
    for (const auto& entry : scores)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(index) + ". " + entry.getPlayerName() + "....." + std::to_string(entry.getScore()));
        text.setCharacterSize(static_cast<unsigned int>(fontSize));
        
        float yPos = tableStartY + ((index - 1) * deltaY);
        text.setPosition(tableStartX, yPos);
        text.setOrigin(0, 0);
        text.setFillColor(sf::Color::White);
        window.draw(text);
        index++;
    }
}

} // namespace ArkanoidGame
