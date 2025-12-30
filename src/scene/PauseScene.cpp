#include "scene/PauseScene.hpp"
#include "scene/MainMenuScene.hpp"
#include "scene/SceneManager.hpp"
#include <iostream>

// =================================================
// CONSTRUCTOR
// =================================================
PauseScene::PauseScene(SceneManager& mgr, sf::RenderWindow& win)
: Scene(mgr)
, window(win)
{
    initAssets();
    initMenuItems();
    initLayout();
}

// =================================================
// LIFECYCLE
// =================================================
void PauseScene::onEnter()
{
    std::cout << "[PauseScene] onEnter\n";
}

void PauseScene::onExit()
{
    std::cout << "[PauseScene] onExit\n";
}

// =================================================
// EVENT
// =================================================
void PauseScene::handleEvent(const sf::Event& e)
{
    if (e.is<sf::Event::KeyPressed>())
    {
        auto key = e.getIf<sf::Event::KeyPressed>()->code;

        // ESC = RESUME
        if (key == sf::Keyboard::Key::Escape)
        {
            manager.pop();
            return;
        }

        handleKeyboard(key);
    }
    else if (e.is<sf::Event::MouseMoved>())
    {
        auto p = e.getIf<sf::Event::MouseMoved>()->position;
        handleMouseMove({(float)p.x, (float)p.y});
    }
    else if (e.is<sf::Event::MouseButtonPressed>())
    {
        auto b = e.getIf<sf::Event::MouseButtonPressed>();
        if (b->button == sf::Mouse::Button::Left)
        {
            handleMouseClick({
                (float)b->position.x,
                (float)b->position.y
            });
        }
    }
}

// =================================================
// RENDER (CHỈ OVERLAY + UI)
// =================================================
void PauseScene::render(sf::RenderWindow& window)
{
    // overlay tối
    UITheme::drawOverlay(window);

    // panel
    if (panelSprite)
        window.draw(*panelSprite);

    // menu items
    for (auto& item : items)
    {
        if (item.sprite)
            window.draw(*item.sprite);

        if (item.text)
            window.draw(*item.text);
    }
}

// =================================================
// INIT ASSETS
// =================================================
void PauseScene::initAssets()
{
    // FONT
    if (!uiFont.openFromFile("asset/ui/fonts/pixel.ttf"))
        std::cout << "[Pause] Font load failed\n";

    // PANEL
    if (!panelTexture.loadFromFile("asset/ui/panels/panel.png"))
        std::cout << "[Pause] Panel load failed\n";

    panelSprite.emplace(panelTexture);

    panelSprite->setOrigin({
        panelTexture.getSize().x * 0.5f,
        panelTexture.getSize().y * 0.5f
    });

    const float targetWidth  = window.getSize().x * 0.8f;
    const float targetHeight = window.getSize().y * 0.8f;

    panelSprite->setScale({
        targetWidth  / panelTexture.getSize().x,
        targetHeight / panelTexture.getSize().y
    });

    panelSprite->setPosition({
        window.getSize().x * 0.5f,
        window.getSize().y * 0.5f
    });
}

// =================================================
// INIT MENU ITEMS
// =================================================
void PauseScene::initMenuItems()
{
    items.clear();

    auto makeItem = [&](const std::string& label, std::function<void()> action)
    {
        Button item;

        // TEXT
        item.text.emplace(uiFont, label, UITheme::FONT_SIZE);
        item.text->setScale({UITheme::TEXT_SCALE_NORMAL, UITheme::TEXT_SCALE_NORMAL});
        item.text->setFillColor(UITheme::TEXT_NORMAL);

        // BUTTON TEXTURES
        if (!item.texNormal.loadFromFile("asset/ui/buttons/btn_normal.png")){
            std::cout << "[Pause] Failed to load btn_normal.png\n";
        }
        if (!item.texHover.loadFromFile("asset/ui/buttons/btn_hover.png")){
            std::cout << "[Pause] Failed to load btn_hover.png\n";
        }
        if (!item.texPressed.loadFromFile("asset/ui/buttons/btn_pressed.png")){
            std::cout << "[Pause] Failed to load btn_pressed.png\n";
        }

        item.sprite.emplace(item.texNormal);
        item.sprite->setScale(UITheme::BTN_SCALE_NORMAL);

        item.action = std::move(action);
        items.push_back(std::move(item));
    };

    // ===== RESUME =====
    makeItem("RESUME", [this]()
    {
        manager.pop();
    });

    // ===== MAIN MENU =====
    makeItem("MAIN MENU", [this]()
    {
        manager.change(
            std::make_unique<MainMenuScene>(manager, window)
        );
    });

    selectedIndex = 0;
}

// =================================================
// LAYOUT
// =================================================
void PauseScene::initLayout()
{
    const float cx  = window.getSize().x * 0.5f;
    const float sy  = window.getSize().y * 0.4f;
    const float gap = 140.f;

    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];
        if (!item.text || !item.sprite)
            continue;

        auto& txt = *item.text;
        auto& btn = *item.sprite;

        // BUTTON
        auto bb = btn.getLocalBounds();
        btn.setOrigin({bb.size.x * 0.5f, bb.size.y * 0.5f});
        btn.setPosition({cx, sy + i * gap});

        // TEXT
        auto tb = txt.getLocalBounds();
        txt.setOrigin({
            tb.position.x + tb.size.x * 0.5f,
            tb.position.y + tb.size.y * 0.5f
        });

        txt.setPosition(btn.getPosition());
    }

    updateVisual();
}

// =================================================
// INPUT: KEYBOARD
// =================================================
void PauseScene::handleKeyboard(sf::Keyboard::Key key)
{
    items[selectedIndex].state = ButtonState::Normal;

    if (key == sf::Keyboard::Key::Up)
    {
        selectedIndex =
            (selectedIndex - 1 + items.size()) % items.size();
    }
    else if (key == sf::Keyboard::Key::Down)
    {
        selectedIndex =
            (selectedIndex + 1) % items.size();
    }
    else if (key == sf::Keyboard::Key::Enter)
    {
        items[selectedIndex].state = ButtonState::Pressed;
        updateVisual();
        items[selectedIndex].action();
        return;
    }

    items[selectedIndex].state = ButtonState::Hover;
    updateVisual();
}

// =================================================
// INPUT: MOUSE MOVE
// =================================================
void PauseScene::handleMouseMove(sf::Vector2f mousePos)
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];
        if (!item.sprite)
            continue;

        if (item.sprite->getGlobalBounds().contains(mousePos))
        {
            if (selectedIndex != i)
            {
                selectedIndex = i;
                item.state = ButtonState::Hover;
                updateVisual();
            }
            return;
        }
    }
}

// =================================================
// INPUT: MOUSE CLICK
// =================================================
void PauseScene::handleMouseClick(sf::Vector2f mousePos)
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];
        if (!item.sprite)
            continue;

        if (item.sprite->getGlobalBounds().contains(mousePos))
        {
            selectedIndex = i;
            item.state = ButtonState::Pressed;
            updateVisual();
            item.action();
            return;
        }
    }
}

// =================================================
// VISUAL UPDATE
// =================================================
void PauseScene::updateVisual()
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];

        // TEXT COLOR
        if (item.text)
        {
            item.text->setFillColor(
                i == selectedIndex
                    ? UITheme::TEXT_HOVER
                    : UITheme::TEXT_NORMAL
            );
        }

        // BUTTON TEXTURE
        if (!item.sprite)
            continue;

        switch (item.state)
        {
        case ButtonState::Normal:
            item.sprite->setTexture(item.texNormal);
            break;

        case ButtonState::Hover:
            item.sprite->setTexture(item.texHover);
            break;

        case ButtonState::Pressed:
            item.sprite->setTexture(item.texPressed);
            break;
        }
    }
}