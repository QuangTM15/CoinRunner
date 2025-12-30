#include "scene/GameOverScene.hpp"
#include "scene/SceneManager.hpp"
#include "scene/PlayScene.hpp"
#include "scene/MainMenuScene.hpp"
#include "ui/UITheme.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

// ===== TROLL TEXT =====
static const std::vector<std::string> TROLL_TEXTS = {
    "Skill issue.",
    "The trap wins this time.",
    "Are you chicken?",
    "Go home to your mom.",
    "Please don't lay eggs please."
};

GameOverScene::GameOverScene(
    SceneManager& mgr,
    sf::RenderWindow& win,
    int lvl,
    int c
)
: Scene(mgr)
, window(win)
, level(lvl)
, coin(c)
{
    initAssets();
    initTexts();
    initMenuItems();
    initLayout();
}

void GameOverScene::onEnter()
{
    std::cout << "[GameOverScene] onEnter\n";
}

void GameOverScene::onExit()
{
    std::cout << "[GameOverScene] onExit\n";
}

// ================= ASSETS =================

void GameOverScene::initAssets()
{
    if (!uiFont.openFromFile("asset/ui/fonts/pixel.ttf"))
        std::cout << "[GameOver] Font load failed\n";

    if (!bgTexture.loadFromFile("asset/ui/panels/Background.png"))
        std::cout << "[GameOver] Background load failed\n";

    bgSprite.emplace(bgTexture);

    bgSprite->setOrigin({
        bgTexture.getSize().x * 0.5f,
        bgTexture.getSize().y * 0.5f
    });

    float sx = (float)window.getSize().x / bgTexture.getSize().x;
    float sy = (float)window.getSize().y / bgTexture.getSize().y;

    bgSprite->setScale({ sx, sy });
    bgSprite->setPosition({
        window.getSize().x * 0.5f,
        window.getSize().y * 0.5f
    });
}

// ================= TEXT =================

void GameOverScene::initTexts()
{
    titleText.emplace(uiFont, "GAME OVER", 64);
    titleText->setFillColor(UITheme::TEXT_HOVER);

    
    coinText.emplace(
        uiFont,
        "COIN: " + std::to_string(coin),
        32
    );
    coinText->setFillColor(sf::Color::White);

    std::srand((unsigned)std::time(nullptr));
    int idx = std::rand() % TROLL_TEXTS.size();
    trollText.emplace(uiFont, TROLL_TEXTS[idx], 24);
    trollText->setFillColor(UITheme::TEXT_NORMAL);
}

// ================= BUTTON =================

void GameOverScene::initMenuItems()
{
    items.clear();

    auto makeItem = [&](const std::string& label, std::function<void()> action)
    {
        Button item;

        item.text.emplace(uiFont, label, UITheme::FONT_SIZE);
        item.text->setScale({
            UITheme::TEXT_SCALE_NORMAL,
            UITheme::TEXT_SCALE_NORMAL
        });
        item.text->setFillColor(UITheme::TEXT_NORMAL);

        if (!item.texNormal.loadFromFile("asset/ui/buttons/btn_normal.png")){
            std::cerr << "[GameOverScene] Cannot load btn_normal.png\n";
        }
        if (!item.texHover.loadFromFile("asset/ui/buttons/btn_hover.png")){
            std::cerr << "[GameOverScene] Cannot load btn_hover.png\n";
        }
        if (!item.texPressed.loadFromFile("asset/ui/buttons/btn_pressed.png")){
            std::cerr << "[GameOverScene] Cannot load btn_pressed.png\n";
        }

        item.sprite.emplace(item.texNormal);
        item.sprite->setScale(UITheme::BTN_SCALE_NORMAL);

        item.action = std::move(action);
        items.push_back(std::move(item));
    };

    makeItem("RETRY", [this]()
    {
        manager.change(
            std::make_unique<PlayScene>(manager, window, level)
        );
    });

    makeItem("MAIN MENU", [this]()
    {
        manager.change(
            std::make_unique<MainMenuScene>(manager, window)
        );
    });
}

// ================= LAYOUT =================

void GameOverScene::initLayout()
{
    const float cx = window.getSize().x * 0.5f;

    // ===== TITLE =====
    if (titleText)
    {
        auto b = titleText->getLocalBounds();
        titleText->setOrigin({
            b.position.x + b.size.x * 0.5f,
            b.position.y + b.size.y * 0.5f
        });
        titleText->setPosition({ cx, 120.f });
    }

    // ===== COIN =====
    if (coinText)
    {
        auto b = coinText->getLocalBounds();
        coinText->setOrigin({
            b.position.x + b.size.x * 0.5f,
            b.position.y + b.size.y * 0.5f
        });
        coinText->setPosition({ cx, 200.f });
    }

    // ===== TROLL TEXT =====
    if (trollText)
    {
        auto b = trollText->getLocalBounds();
        trollText->setOrigin({
            b.position.x + b.size.x * 0.5f,
            b.position.y + b.size.y * 0.5f
        });
        trollText->setPosition({ cx, 250.f });
    }

    // ===== BUTTONS =====
    const float btnY = 360.f;
    const float gap  = 130.f;

    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];
        if (!item.sprite || !item.text)
            continue;

        auto& btn = *item.sprite;
        auto& txt = *item.text;

        // ---- BUTTON ----
        auto bb = btn.getLocalBounds();
        btn.setOrigin({
            bb.position.x + bb.size.x * 0.5f,
            bb.position.y + bb.size.y * 0.5f
        });
        btn.setPosition({ cx, btnY + i * gap });

        // ---- TEXT (CENTER IN BUTTON) ----
        auto tb = txt.getLocalBounds();
        txt.setOrigin({
            tb.position.x + tb.size.x * 0.5f,
            tb.position.y + tb.size.y * 0.5f
        });
        txt.setPosition(btn.getPosition());
    }

    updateVisual();
}

void GameOverScene::updateVisual()
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];

        if (item.text)
        {
            item.text->setFillColor(
                i == selectedIndex
                    ? UITheme::TEXT_HOVER
                    : UITheme::TEXT_NORMAL
            );
        }

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

// ================= LOOP =================

void GameOverScene::handleEvent(const sf::Event& e)
{
    if (e.is<sf::Event::KeyPressed>())
    {
        auto key = e.getIf<sf::Event::KeyPressed>()->code;

        items[selectedIndex].state = ButtonState::Normal;

        if (key == sf::Keyboard::Key::Up)
            selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
        else if (key == sf::Keyboard::Key::Down)
            selectedIndex = (selectedIndex + 1) % items.size();
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

void GameOverScene::update(float) {}

void GameOverScene::render(sf::RenderWindow& window)
{
    if (bgSprite)
        window.draw(*bgSprite);

    if (titleText) window.draw(*titleText);
    if (coinText)  window.draw(*coinText);
    if (trollText) window.draw(*trollText);

    for (auto& item : items)
    {
        window.draw(*item.sprite);
        window.draw(*item.text);
    }
}

void GameOverScene::handleMouseMove(sf::Vector2f mousePos)
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

void GameOverScene::handleMouseClick(sf::Vector2f mousePos)
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
            item.action();
            return;
        }
    }
}