#pragma once
#include "Scene.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

class LevelSelectScene : public Scene {
public:
    LevelSelectScene(SceneManager& mgr, sf::RenderWindow& window);

    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;
    void onExit() override;

private:
    // ===== INIT =====
    void initAssets();
    void initLevelItems();
    void initLayout();
    void updateVisual();

    // ===== INPUT =====
    void handleKeyboard(sf::Keyboard::Key key);
    void handleMouseMove(sf::Vector2f pos);
    void handleMouseClick(sf::Vector2f pos);

private:
    sf::RenderWindow& window;

    sf::Font font;

    struct LevelItem {
        int level;
        bool unlocked;
        sf::Text text;
    };

    std::vector<LevelItem> items;
    std::optional<sf::Text> backText;

    int maxUnlockedLevel = 1;
    int selectedIndex = 0; 
};