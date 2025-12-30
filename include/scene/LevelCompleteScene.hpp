#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include "ui/Button.hpp"

class LevelCompleteScene : public Scene {
public:
    LevelCompleteScene(
        SceneManager& mgr,
        sf::RenderWindow& window,
        int currentLevel,
        int coin
    );

    void onEnter() override;
    void onExit() override;

    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void initAssets();
    void initTexts();
    void initMenuItems();
    void initLayout();
    void updateVisual();

    void handleMouseMove(sf::Vector2f pos);
    void handleMouseClick(sf::Vector2f pos);

private:
    sf::RenderWindow& window;

    int level;
    int coin;

    sf::Font uiFont;

    sf::Texture bgTexture;
    std::optional<sf::Sprite> bgSprite;

    std::optional<sf::Text> titleText;
    std::optional<sf::Text> coinText;

    std::vector<Button> items;
    int selectedIndex = 0;
};