#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <SFML/Audio/Music.hpp>

class EndGameScene : public Scene {
public:
    EndGameScene(SceneManager& mgr, sf::RenderWindow& window);

    void onEnter() override;
    void onExit() override;

    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    enum class State {
        ShowPage,
        FadeOut,
        Exit
    };

    struct Page {
        std::string en;
        std::string vi;
    };

    void initAssets();
    void initPages();
    void updateTexts();

private:
    sf::RenderWindow& window;

    // ---- assets ----
    sf::Font uiFont;
    sf::RectangleShape blackBg;

    std::optional<sf::Text> textEN;
    std::optional<sf::Text> textVI;

    // ---- flow ----
    std::vector<Page> pages;
    int currentPage = 0;

    State state = State::ShowPage;
    float timer = 0.f;

    // timing
    const float PAGE_DURATION = 2.8f;
    const float FADE_DURATION = 1.2f;

    float fadeAlpha = 0.f;

    sf::Music endMusic;
};