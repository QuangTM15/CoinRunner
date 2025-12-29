#include "system/AudioManager.hpp"
#include <iostream>

AudioManager& AudioManager::get()
{
    static AudioManager instance;
    return instance;
}

void AudioManager::init()
{
    // ---- MUSIC ----
    if (!bg.openFromFile("asset/musics/MusicBackGround.mp3"))
        std::cout << "[Audio] Failed to load MusicBackGround.mp3\n";

    bg.setLooping(true);
    bg.setVolume(35.f);

    // ---- SFX ----
    if (!bufJump.loadFromFile("asset/sounds/jump.wav"))
        std::cout << "[Audio] Failed to load jump.wav\n";
    if (!bufCoin.loadFromFile("asset/sounds/coin.wav"))
        std::cout << "[Audio] Failed to load coin.wav\n";
    if (!bufHit.loadFromFile("asset/sounds/hit.wav"))
        std::cout << "[Audio] Failed to load hit.wav\n";

    // ⚠️ SFML 3: Sound phải tạo SAU khi có buffer
    sJump.emplace(bufJump);
    sCoin.emplace(bufCoin);
    sHit.emplace(bufHit);

    sJump->setVolume(100.f);
    sCoin->setVolume(90.f);
    sHit->setVolume(200.f);
}

void AudioManager::setMusicEnabled(bool on)
{
    musicEnabled = on;
    if (!musicEnabled)
        stopBackground();
}

bool AudioManager::isMusicEnabled() const
{
    return musicEnabled;
}

void AudioManager::playBackground()
{
    if (!musicEnabled) return;
    if (bg.getStatus() != sf::Music::Status::Playing)
        bg.play();
}

void AudioManager::stopBackground()
{
    bg.stop();
}

void AudioManager::playJump()
{
    sJump->play();
}

void AudioManager::playCoin()
{
    sCoin->play();
}

void AudioManager::playHit()
{
    sHit->play();
}