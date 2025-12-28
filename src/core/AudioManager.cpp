#include "core/AudioManager.hpp"
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

    bg.setLooping(true); // ✅ SFML 3

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
}

void AudioManager::setEnabled(bool on)
{
    enabled = on;
    if (!enabled)
    {
        stopBackground();
        if (sJump) sJump->stop();
        if (sCoin) sCoin->stop();
        if (sHit)  sHit->stop();
    }
}

bool AudioManager::isEnabled() const
{
    return enabled;
}

void AudioManager::playBackground()
{
    if (!enabled) return;
    if (bg.getStatus() != sf::Music::Status::Playing)
        bg.play();
}

void AudioManager::stopBackground()
{
    bg.stop();
}

void AudioManager::playJump()
{
    if (!enabled || !sJump) return;
    sJump->play();
}

void AudioManager::playCoin()
{
    if (!enabled || !sCoin) return;
    sCoin->play();
}

void AudioManager::playHit()
{
    if (!enabled || !sHit) return;
    sHit->play();
}