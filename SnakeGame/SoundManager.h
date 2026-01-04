#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include "Constants.h"

namespace ArkanoidGame
{

// Forward declaration (чтобы избежать циклических зависимостей)
class MenuState;

// Класс для управления всеми звуками и музыкой в игре
class SoundManager
{
public:
    SoundManager(MenuState* menuState);  // Конструктор принимает указатель на MenuState
    void playSessionEnd();  // Звук при окончании игры (GAME_OVER)
    void playBackgroundMusic();  // Фоновая музыка
    void stopBackgroundMusic();  // Остановка фоновой музыки
    void playBlockHit();  // Звук удара по блоку
    void playMenuHover();  // Звук наведения в меню
    void playEnter();  // Звук нажатия Enter
    
private:
    // Указатель на MenuState для проверки настроек звука/музыки
    MenuState* menuState;
    // Буферы для звуковых эффектов
    sf::SoundBuffer sessionEndBuffer;
    sf::SoundBuffer blockHitBuffer;
    sf::SoundBuffer menuHoverBuffer;
    sf::SoundBuffer enterSoundBuffer;
    
    // Объекты для воспроизведения звуковых эффектов
    sf::Sound sessionEndSound;
    sf::Sound blockHitSound;
    sf::Sound menuHoverSound;
    sf::Sound enterSound;
    
    // Фоновая музыка (использует sf::Music для стриминга)
    sf::Music backgroundMusic;
};

} // namespace ArkanoidGame
