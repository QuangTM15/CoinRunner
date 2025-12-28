#pragma once
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <optional>

class MainMenuScene : public Scene {
public:
    MainMenuScene(SceneManager& mgr, sf::RenderWindow& window);

    // Scene lifecycle
    void onEnter() override;
    void onExit() override;

    // Core methods
    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    /// button states
    enum class ButtonState {
        Normal,
        Hover,
        Pressed
    };

    /// menu item structure
    struct MenuItem {
        // TEXT (lazy)
        std::optional<sf::Text> text;

        // SPRITE BUTTON (lazy)
        std::optional<sf::Sprite> sprite;

        // TEXTURES (chỉ data, OK default ctor)
        sf::Texture texNormal;
        sf::Texture texHover;
        sf::Texture texPressed;

        ButtonState state = ButtonState::Normal;
        std::function<void()> action;
    };

private:
    // ---- Init ----
    void initAssets();
    void initMenuItems();
    void initLayout();

    // ---- Input (chưa bật) ----
    void handleKeyboard(sf::Keyboard::Key key);
    void handleMouseMove(sf::Vector2f pos);
    void handleMouseClick(sf::Vector2f pos);

    void updateVisual();

private:
    sf::RenderWindow& window;

    // UI assets (lazy)
    std::optional<sf::Font> uiFont;
    std::optional<sf::Texture> panelTexture;
    std::optional<sf::Sprite>  panelSprite;

    std::vector<MenuItem> items;
    int selectedIndex = 0;
};