#include "SoundManager.h"
#include "UI.h"
#include <cassert>

namespace SnakeGame
{

SoundManager::SoundManager(MenuState* menuState)
{
    this->menuState = menuState;
    
    assert(sessionEndBuffer.loadFromFile(RESOURCES_PATH + "Maodin204__Lose.wav"));
    sessionEndSound.setBuffer(sessionEndBuffer);

    assert(appleEatBuffer.loadFromFile(RESOURCES_PATH + "Owlstorm__Snake_hit.wav"));
    appleEatSound.setBuffer(appleEatBuffer);

    assert(menuHoverBuffer.loadFromFile(RESOURCES_PATH + "Theevilsocks__menu-hover.wav"));
    menuHoverSound.setBuffer(menuHoverBuffer);

    assert(enterSoundBuffer.loadFromFile(RESOURCES_PATH + "Timgormly__Enter.wav"));
    enterSound.setBuffer(enterSoundBuffer);

    assert(backgroundMusic.openFromFile(RESOURCES_PATH + "Clinthammer__Background_Music.wav"));
}

void SoundManager::playSessionEnd()
{
    if (menuState && menuState->getSoundEnabled())
        sessionEndSound.play();
}

void SoundManager::playBackgroundMusic()
{
    if (menuState && menuState->getMusicEnabled())
    {
        backgroundMusic.setVolume(30.0f);
        backgroundMusic.play();
        backgroundMusic.setLoop(true);
    }
}

void SoundManager::stopBackgroundMusic()
{
    backgroundMusic.stop();
}

void SoundManager::playAppleEat()
{
    if (menuState && menuState->getSoundEnabled())
    {
        appleEatSound.setVolume(100.0f);
        appleEatSound.play();
    }
}

void SoundManager::playMenuHover()
{
    if (menuState && menuState->getSoundEnabled())
        menuHoverSound.play();
}

void SoundManager::playEnter()
{
    if (menuState && menuState->getSoundEnabled())
    {
        enterSound.stop();
        enterSound.setVolume(100.0f);
        enterSound.play();
    }
}

} // namespace SnakeGame
