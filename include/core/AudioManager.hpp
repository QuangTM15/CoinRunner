#pragma once
#include <SFML/Audio.hpp>
#include <optional>

class AudioManager
{
public:
    static AudioManager& get();

    void init();

    // 🎵 MUSIC
    void setMusicEnabled(bool on);
    bool isMusicEnabled() const;

    void playBackground();
    void stopBackground();

    void playJump();
    void playCoin();
    void playHit();

private:
    AudioManager() = default;

    bool musicEnabled = true;

    sf::Music bg;

    sf::SoundBuffer bufJump;
    sf::SoundBuffer bufCoin;
    sf::SoundBuffer bufHit;

    std::optional<sf::Sound> sJump;
    std::optional<sf::Sound> sCoin;
    std::optional<sf::Sound> sHit;
};
