#include "scene/MainMenuScene.hpp"
#include "scene/PlayScene.hpp"
#include "scene/SceneManager.hpp"
#include <iostream>
#include <system/AudioManager.hpp>
#include <scene/LevelSelectScene.hpp>
#include <ui/UITheme.hpp>

MainMenuScene::MainMenuScene(SceneManager& mgr, sf::RenderWindow& win)
: Scene(mgr)
, window(win)
{
    initAssets();
    initMenuItems();
    initLayout();
}

void MainMenuScene::onEnter()
{
    std::cout << "[MainMenuScene] onEnter\n";
}

void MainMenuScene::onExit()
{
    std::cout << "[MainMenuScene] onExit\n";
}

void MainMenuScene::handleEvent(const sf::Event& e)
{
    if (e.is<sf::Event::KeyPressed>())
    {
        handleKeyboard(e.getIf<sf::Event::KeyPressed>()->code);
    }
    else if (e.is<sf::Event::MouseMoved>())
    {
        auto p = e.getIf<sf::Event::MouseMoved>()->position;
        handleMouseMove({ (float)p.x, (float)p.y });
    }
    else if (e.is<sf::Event::MouseButtonPressed>())
    {
        auto b = e.getIf<sf::Event::MouseButtonPressed>();
        if (b->button == sf::Mouse::Button::Left)
            handleMouseClick({ (float)b->position.x, (float)b->position.y });
    }
}

void MainMenuScene::update(float) {}

void MainMenuScene::render(sf::RenderWindow& window)
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
            window.draw(*item.text);
    }
}

// ================= INIT =================

void MainMenuScene::initAssets()
{
    // ---- FONT ----
    if (!uiFont.openFromFile("asset/ui/fonts/pixel.ttf"))
        std::cout << "[Menu] Font load failed\n";

    // ---- PANEL TEXTURE ----
    if (!panelTexture.loadFromFile("asset/ui/panels/panel.png"))
        std::cout << "[Menu] Panel load failed\n";

    panelSprite.emplace(panelTexture);

    // origin giữa panel
    panelSprite->setOrigin({
        panelTexture.getSize().x * 0.5f,
        panelTexture.getSize().y * 0.5f
    });

    // ======SCALE PANEL ======
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

void MainMenuScene::initMenuItems()
{
    items.clear();

    constexpr unsigned int CHAR_SIZE = UITheme::FONT_SIZE;
    const sf::Vector2f TEXT_SCALE{UITheme::TEXT_SCALE_NORMAL, UITheme::TEXT_SCALE_NORMAL};
    const sf::Vector2f BTN_SCALE{UITheme::BTN_SCALE_NORMAL.x, UITheme::BTN_SCALE_NORMAL.y};

    auto makeItem = [&](const std::string& label, std::function<void()> action)
    {
        MenuItem item;

        item.text.emplace(uiFont, label, CHAR_SIZE);
        item.text->setScale(TEXT_SCALE);
        item.text->setFillColor(UITheme::TEXT_NORMAL);

        if (!item.texNormal.loadFromFile("asset/ui/buttons/btn_normal.png"))
        {
            std::cout << "[Menu] Failed to load btn_normal.png\n";
        }
        if (!item.texHover.loadFromFile("asset/ui/buttons/btn_hover.png"))
        {
            std::cout << "[Menu] Failed to load btn_hover.png\n";
        }
        if (!item.texPressed.loadFromFile("asset/ui/buttons/btn_pressed.png"))
        {
            std::cout << "[Menu] Failed to load btn_pressed.png\n";
        }

        item.sprite.emplace(item.texNormal);
        item.sprite->setScale(BTN_SCALE);

        item.action = std::move(action);
        items.push_back(std::move(item));
    };

    makeItem("PLAY", [this]()
    {
        manager.change(std::make_unique<PlayScene>(manager, window));
    });

    makeItem("LEVEL SELECT", [this](){
        manager.change(std::make_unique<LevelSelectScene>(manager, window));
    });

    makeItem("MUSIC: ON", [this]()
    {
        musicOn = !musicOn;

        // 🔥 DÒNG QUYẾT ĐỊNH: chỉ tắt / bật NHẠC NỀN
        AudioManager::get().setMusicEnabled(musicOn);

        auto& t = *items[selectedIndex].text;
        t.setString(musicOn ? "MUSIC: ON" : "MUSIC: OFF");
    });

    makeItem("EXIT", [this]()
    {
        window.close();
    });
}

void MainMenuScene::initLayout()
{
    const float cx  = window.getSize().x * 0.5f;
    const float sy  = window.getSize().y * 0.2f;
    const float gap = 150.f; // tách các button ra rõ hơn

    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];
        if (!item.text || !item.sprite)
            continue;

        auto& txt = *item.text;
        auto& btn = *item.sprite;

        // ===== BUTTON =====
        sf::FloatRect bb = btn.getLocalBounds();
        btn.setOrigin({bb.size.x * 0.5f, bb.size.y * 0.5f});
        btn.setPosition({cx, sy + i * gap});

        // ===== TEXT =====
        sf::FloatRect tb = txt.getLocalBounds();
        txt.setOrigin(
            {tb.position.x + tb.size.x * 0.5f,
            tb.position.y + tb.size.y * 0.5f
        });

        // 👉 text luôn nằm giữa button
        txt.setPosition(btn.getPosition());
    }

    updateVisual();
}

// ================= INPUT =================

void MainMenuScene::handleKeyboard(sf::Keyboard::Key key)
{
    items[selectedIndex].state = ButtonState::Normal;

    if (key == sf::Keyboard::Key::Up)
    {
        selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
    }
    else if (key == sf::Keyboard::Key::Down)
    {
        selectedIndex = (selectedIndex + 1) % items.size();
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

void MainMenuScene::handleMouseMove(sf::Vector2f mousePos)
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];
        if (!item.sprite) continue;

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

void MainMenuScene::handleMouseClick(sf::Vector2f mousePos)
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];
        if (!item.sprite) continue;

        if (item.sprite->getGlobalBounds().contains(mousePos))
        {
            selectedIndex = i;
            item.state = ButtonState::Pressed;
            updateVisual();

            // chạy action
            item.action();
            return;
        }
    }
}

void MainMenuScene::updateVisual()
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];

        // ----- TEXT COLOR -----
        if (item.text)
        {
            item.text->setFillColor(
                i == selectedIndex
                    ? UITheme::TEXT_HOVER
                    : UITheme::TEXT_NORMAL
            );
        }

        // ----- BUTTON TEXTURE -----
        if (!item.sprite) continue;

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