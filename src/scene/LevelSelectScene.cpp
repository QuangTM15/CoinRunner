#include "scene/LevelSelectScene.hpp"
#include "scene/SceneManager.hpp"
#include "scene/MainMenuScene.hpp"
#include "scene/PlayScene.hpp"
#include "system/SaveManager.hpp"
#include <ui/UITheme.hpp>

#include <iostream>

LevelSelectScene::LevelSelectScene(SceneManager& mgr, sf::RenderWindow& win)
: Scene(mgr)
, window(win)
{}

void LevelSelectScene::onEnter()
{
    std::cout << "[LevelSelectScene] onEnter\n";

    maxUnlockedLevel = SaveManager::get().getMaxUnlockedLevel();
    selectedIndex = 0;

    initAssets();
    initLevelItems();
    initLayout();
    updateVisual();
}

void LevelSelectScene::onExit()
{
    std::cout << "[LevelSelectScene] onExit\n";
}

// ================= INIT =================

void LevelSelectScene::initAssets()
{
    if (!uiFont.openFromFile("asset/ui/fonts/pixel.ttf"))
        std::cout << "[LevelSelect] Font load FAILED\n";

    // ---- PANEL TEXTURE ----
    if (!panelTexture.loadFromFile("asset/ui/panels/panel.png"))
        std::cout << "[LevelSelect] Panel load FAILED\n";

    panelSprite.emplace(panelTexture);

    // origin giữa panel
    panelSprite->setOrigin({
        panelTexture.getSize().x * 0.5f,
        panelTexture.getSize().y * 0.5f
    });

    // ===== SCALE PANEL =====
    const float targetWidth  = window.getSize().x * 1.2f;
    const float targetHeight = window.getSize().y * 1.2f;

    const sf::Vector2f panelSize(
        static_cast<float>(panelTexture.getSize().x),
        static_cast<float>(panelTexture.getSize().y)
    );

    panelSprite->setScale({
        targetWidth  / panelSize.x,
        targetHeight / panelSize.y
    });

    // position giữa màn hình
    panelSprite->setPosition({
        window.getSize().x * 0.5f,
        window.getSize().y * 0.5f
    });
}

void LevelSelectScene::initLevelItems()
{
    items.clear();
    items.reserve(3);

    constexpr unsigned int CHAR_SIZE = UITheme::FONT_SIZE;
    const sf::Vector2f TEXT_SCALE{UITheme::TEXT_SCALE_NORMAL, UITheme::TEXT_SCALE_NORMAL};
    const sf::Vector2f BTN_SCALE{UITheme::BTN_SCALE_NORMAL.x, UITheme::BTN_SCALE_NORMAL.y};

    for (int i = 1; i <= 3; ++i)
    {
        LevelItem item;
        item.level = i;
        item.unlocked = (i <= maxUnlockedLevel);

        // ---- TEXT ----
        std::string label = "LEVEL " + std::to_string(i);
        item.text.emplace(uiFont, label, CHAR_SIZE);
        item.text->setScale(TEXT_SCALE);

        // ---- TEXTURES ----
        if (!item.texNormal.loadFromFile("asset/ui/buttons/btn_normal.png"))
        {
            std::cout << "[LevelSelect] Failed to load btn_normal.png\n";
        }
        if (!item.texHover.loadFromFile("asset/ui/buttons/btn_hover.png"))
        {
            std::cout << "[LevelSelect] Failed to load btn_hover.png\n";
        }
        if (!item.texPressed.loadFromFile("asset/ui/buttons/btn_pressed.png"))
        {
            std::cout << "[LevelSelect] Failed to load btn_pressed.png\n";
        }
        if (!item.texLocked.loadFromFile("asset/ui/buttons/btn_lock.png"))
        {
            std::cout << "[LevelSelect] Failed to load btn_lock.png\n";
        }

        // ---- SPRITE ----
        if (item.unlocked)
        {
            item.sprite.emplace(item.texNormal);
            item.state = ButtonState::Normal;
        }
        else
        {
            item.sprite.emplace(item.texLocked);
            item.state = ButtonState::Locked;
        }

        item.sprite->setScale(BTN_SCALE);

        items.push_back(std::move(item));
    }

    // ===== BACK BUTTON =====
    backItem.level = -1;
    backItem.unlocked = true;

    backItem.text.emplace(uiFont, "BACK", CHAR_SIZE);
    backItem.text->setScale(TEXT_SCALE);

    if (!backItem.texNormal.loadFromFile("asset/ui/buttons/btn_normal.png"))
    {
        std::cout << "[LevelSelect] Failed to load btn_normal.png\n";
    }
    if (!backItem.texHover.loadFromFile("asset/ui/buttons/btn_hover.png"))
    {
        std::cout << "[LevelSelect] Failed to load btn_hover.png\n";
    }
    if (!backItem.texPressed.loadFromFile("asset/ui/buttons/btn_pressed.png"))
    {
        std::cout << "[LevelSelect] Failed to load btn_pressed.png\n";
    }

    backItem.sprite.emplace(backItem.texNormal);
    backItem.sprite->setScale(BTN_SCALE);
}

void LevelSelectScene::initLayout()
{
    const float cx = window.getSize().x * 0.5f;
    const float sy = window.getSize().y * 0.25f;
    const float gap = 120.f;

    // ---- LEVELS ----
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];

        auto& btn = *item.sprite;
        auto bb = btn.getLocalBounds();
        btn.setOrigin({bb.size.x * 0.5f, bb.size.y * 0.5f});
        btn.setPosition({cx, sy + i * gap});

        auto& txt = *item.text;
        auto tb = txt.getLocalBounds();
        txt.setOrigin({
            tb.position.x + tb.size.x * 0.5f,
            tb.position.y + tb.size.y * 0.5f
        });
        txt.setPosition(btn.getPosition());
    }

    // ---- BACK ----
    {
        auto& btn = *backItem.sprite;
        auto bb = btn.getLocalBounds();
        btn.setOrigin({bb.size.x * 0.5f, bb.size.y * 0.5f});
        btn.setPosition({
            cx,
            window.getSize().y - 120.f
        });

        auto& txt = *backItem.text;
        auto tb = txt.getLocalBounds();
        txt.setOrigin({
            tb.position.x + tb.size.x * 0.5f,
            tb.position.y + tb.size.y * 0.5f
        });
        txt.setPosition(btn.getPosition());
    }

    updateVisual();
}

void LevelSelectScene::updateVisual()
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];
        if (item.state == ButtonState::Locked)
        {
            item.sprite->setTexture(item.texLocked);
            item.text->setFillColor(UITheme::TEXT_LOCKED);
            continue;
        }

        item.text->setFillColor(
            i == selectedIndex
                ? UITheme::TEXT_HOVER
                : UITheme::TEXT_NORMAL
        );

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
        default: break;
        }
    }

    // ---- BACK ----
    backItem.text->setFillColor(
        selectedIndex == (int)items.size()
            ? UITheme::TEXT_HOVER
            : UITheme::TEXT_NORMAL
    );

    switch (backItem.state)
    {
    case ButtonState::Normal:
        backItem.sprite->setTexture(backItem.texNormal);
        break;
    case ButtonState::Hover:
        backItem.sprite->setTexture(backItem.texHover);
        break;
    case ButtonState::Pressed:
        backItem.sprite->setTexture(backItem.texPressed);
        break;
    default: break;
    }
}

// ================= INPUT =================

void LevelSelectScene::handleEvent(const sf::Event& e)
{
    if (e.is<sf::Event::KeyPressed>())
    {
        handleKeyboard(e.getIf<sf::Event::KeyPressed>()->code);
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
            handleMouseClick({(float)b->position.x, (float)b->position.y});
    }
}

void LevelSelectScene::handleKeyboard(sf::Keyboard::Key key)
{
    int total = items.size() + 1; // + BACK

    // ---------- MOVE SELECTION ----------
    if (key == sf::Keyboard::Key::Up)
    {
        do {
            selectedIndex = (selectedIndex - 1 + total) % total;
        }
        while (
            selectedIndex < (int)items.size() &&
            !items[selectedIndex].unlocked
        );
    }
    else if (key == sf::Keyboard::Key::Down)
    {
        do {
            selectedIndex = (selectedIndex + 1) % total;
        }
        while (
            selectedIndex < (int)items.size() &&
            !items[selectedIndex].unlocked
        );
    }
    else if (key == sf::Keyboard::Key::Enter)
    {
        // ---- BACK ----
        if (selectedIndex == (int)items.size())
        {
            manager.change(
                std::make_unique<MainMenuScene>(manager, window)
            );
            return;
        }

        // ---- LEVEL ----
        auto& item = items[selectedIndex];
        if (item.unlocked)
        {
            manager.change(
                std::make_unique<PlayScene>(manager, window, item.level)
            );
            return;
        }
    }
    else if (key == sf::Keyboard::Key::Escape)
    {
        manager.change(
            std::make_unique<MainMenuScene>(manager, window)
        );
        return;
    }

    // ---------- RESET STATES ----------
    for (auto& it : items)
    {
        if (it.unlocked)
            it.state = ButtonState::Normal;
    }
    backItem.state = ButtonState::Normal;

    // ---------- SET HOVER ----------
    if (selectedIndex == (int)items.size())
    {
        backItem.state = ButtonState::Hover;
    }
    else
    {
        items[selectedIndex].state = ButtonState::Hover;
    }

    updateVisual();
}

void LevelSelectScene::handleMouseMove(sf::Vector2f pos)
{
    // ---- LEVEL ITEMS ----
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];

        if (item.sprite &&
            item.sprite->getGlobalBounds().contains(pos))
        {
            // ❌ level lock thì không hover
            if (!item.unlocked)
                return;

            selectedIndex = i;

            // reset state
            for (auto& it : items)
                if (it.unlocked)
                    it.state = ButtonState::Normal;

            item.state = ButtonState::Hover;
            backItem.state = ButtonState::Normal;

            updateVisual();
            return;
        }
    }

    // ---- BACK ----
    if (backItem.sprite &&
        backItem.sprite->getGlobalBounds().contains(pos))
    {
        selectedIndex = items.size();

        for (auto& it : items)
            if (it.unlocked)
                it.state = ButtonState::Normal;

        backItem.state = ButtonState::Hover;

        updateVisual();
        return;
    }
}

void LevelSelectScene::handleMouseClick(sf::Vector2f pos)
{
    // ---- BACK ----
    if (backItem.sprite &&
        backItem.sprite->getGlobalBounds().contains(pos))
    {
        manager.change(
            std::make_unique<MainMenuScene>(manager, window)
        );
        return;
    }

    // ---- LEVEL ----
    for (auto& item : items)
    {
        if (item.sprite &&
            item.sprite->getGlobalBounds().contains(pos))
        {
            // không click level bị lock
            if (!item.unlocked)
                return;

            manager.change(
                std::make_unique<PlayScene>(manager, window, item.level)
            );
            return;
        }
    }
}

// ================= UPDATE / RENDER =================

void LevelSelectScene::update(float) {}

void LevelSelectScene::render(sf::RenderWindow& window)
{
    // vẽ panel
    if (panelSprite)
        window.draw(*panelSprite);
    // overlay tối rất nhẹ
    UITheme::drawOverlay(window);

    // vẽ items
    for (auto& item : items)
    {
        if (item.sprite)
            window.draw(*item.sprite);

        if (item.text)
        {
            UITheme::drawTextWithShadow(window, *item.text);
        }
    }

    if (backItem.sprite) {
        window.draw(*backItem.sprite);
    }
    if (backItem.text) UITheme::drawTextWithShadow(window, *backItem.text);
}