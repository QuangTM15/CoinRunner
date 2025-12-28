#pragma once
#include <SFML/Audio.hpp>
#include <optional>

class AudioManager
{
public:
    static AudioManager& get();

    void init();

    void setEnabled(bool on);
    bool isEnabled() const;

    void playBackground();
    void stopBackground();

    void playJump();
    void playCoin();
    void playHit();

private:
    AudioManager() = default;

    bool enabled = true;

    sf::Music bg;

    sf::SoundBuffer bufJump;
    sf::SoundBuffer bufCoin;
    sf::SoundBuffer bufHit;

    std::optional<sf::Sound> sJump;
    std::optional<sf::Sound> sCoin;
    std::optional<sf::Sound> sHit;
};
