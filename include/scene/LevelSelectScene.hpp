#pragma once
#include "Scene.hpp"

class LevelSelectScene : public Scene {
public:
    LevelSelectScene(SceneManager& mgr, sf::RenderWindow& window);

    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;
    void onExit() override;

private:
    sf::RenderWindow& window;
    sf::Font font;
    std::vector<sf::Text> levelTexts;
    std::optional<sf::Text> backText;
    int maxUnlockedLevel = 1;
};