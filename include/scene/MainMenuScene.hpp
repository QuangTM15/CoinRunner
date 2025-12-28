#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <optional>

class MainMenuScene : public Scene {
public:
    MainMenuScene(SceneManager& mgr, sf::RenderWindow& window);

    void onEnter() override;
    void onExit() override;

    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:

    enum class ButtonState {
        Normal,
        Hover,
        Pressed
    };

    struct MenuItem
    {
        std::optional<sf::Text>   text;
        std::optional<sf::Sprite> sprite;

        sf::Texture texNormal;
        sf::Texture texHover;
        sf::Texture texPressed;

        ButtonState state = ButtonState::Normal;

        std::function<void()> action;
    };

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

    std::vector<MenuItem> items;
    int selectedIndex = 0;
    bool musicOn = true;
};