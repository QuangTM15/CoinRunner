#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <functional>
#include <ui/UITheme.hpp>
#include <ui/Button.hpp>

class PlayScene;

class PauseScene : public Scene {
public:
    PauseScene(SceneManager& mgr, sf::RenderWindow& win);

    void onEnter() override;
    void onExit() override;
    void handleEvent(const sf::Event& e) override;
    void update(float) override {}
    void render(sf::RenderWindow& window) override;

private:
    void initAssets();
    void initMenuItems();
    void initLayout();
    void updateVisual();

    void handleKeyboard(sf::Keyboard::Key key);
    void handleMouseMove(sf::Vector2f mousePos);
    void handleMouseClick(sf::Vector2f mousePos);

private:
    sf::RenderWindow& window;
    PlayScene* playScene; // 👈 QUAN TRỌNG

    sf::Font uiFont;
    sf::Texture panelTexture;
    std::optional<sf::Sprite> panelSprite;

    std::vector<Button> items;
    int selectedIndex = 0;
};