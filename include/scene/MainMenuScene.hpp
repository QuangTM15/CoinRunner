#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <optional>
#include "ui/Button.hpp"

class MainMenuScene : public Scene {
public:
    MainMenuScene(SceneManager& mgr, sf::RenderWindow& window);

    void onEnter() override;
    void onExit() override;

    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void initAssets();
    void initMenuItems();
    void initLayout();

    void handleKeyboard(sf::Keyboard::Key key);
    void handleMouseMove(sf::Vector2f pos);
    void handleMouseClick(sf::Vector2f pos);
    void updateVisual();

private:
    sf::RenderWindow& window;

    sf::Font uiFont;

    sf::Texture panelTexture;
    std::optional<sf::Sprite> panelSprite;

    std::vector<Button> items;
    int selectedIndex = 0;
    bool musicOn = true;
};