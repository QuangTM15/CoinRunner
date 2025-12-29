#pragma once
#include "Scene.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

enum class ButtonState {
    Normal,
    Hover,
    Pressed,
    Locked
};

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
    sf::Font uiFont;
    // ---- PANEL ----
    sf::Texture panelTexture;
    std::optional<sf::Sprite> panelSprite;

    struct LevelItem {
        int level = 1;
        bool unlocked = false;

        sf::Texture texNormal;
        sf::Texture texHover;
        sf::Texture texPressed;
        sf::Texture texLocked;

        std::optional<sf::Sprite> sprite;
        std::optional<sf::Text>   text;

        ButtonState state = ButtonState::Normal;
    };

    std::vector<LevelItem> items;

    // BACK button
    LevelItem backItem;

    int maxUnlockedLevel = 1;
    int selectedIndex = 0; // 0..items-1, back = items.size()
};