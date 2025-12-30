#include "scene/LevelCompleteScene.hpp"
#include "scene/SceneManager.hpp"
#include "scene/PlayScene.hpp"
#include "scene/MainMenuScene.hpp"
#include "ui/UITheme.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

LevelCompleteScene::LevelCompleteScene(
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


void LevelCompleteScene::onEnter()
{
    std::cout << "[LevelCompleteScene] onEnter\n";
}

void LevelCompleteScene::onExit()
{
    std::cout << "[LevelCompleteScene] onExit\n";
}

// ================= ASSETS =================

void LevelCompleteScene::initAssets()
{
    if (!uiFont.openFromFile("asset/ui/fonts/pixel.ttf"))
        std::cout << "[LevelCompleteScene] Font load failed\n";

    if (!bgTexture.loadFromFile("asset/ui/panels/Background.png"))
        std::cout << "[LevelCompleteScene] Background load failed\n";

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

void LevelCompleteScene::initTexts()
{
    titleText.emplace(uiFont, "LEVEL COMPLETE", 56);
    titleText->setFillColor(UITheme::TEXT_HOVER);

    coinText.emplace(
        uiFont,
        "COIN: " + std::to_string(coin),
        32
    );
    coinText->setFillColor(sf::Color::White);
}

// ================= BUTTON =================

void LevelCompleteScene::initMenuItems()
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
            std::cerr << "[LevelCompleteScene] Cannot load btn_normal.png\n";
        } 
        if (!item.texHover.loadFromFile("asset/ui/buttons/btn_hover.png")){
            std::cerr << "[LevelCompleteScene] Cannot load btn_hover.png\n";
        }

        item.sprite.emplace(item.texNormal);
        item.sprite->setScale(UITheme::BTN_SCALE_NORMAL);

        item.action = std::move(action);
        items.push_back(std::move(item));
    };

    makeItem("NEXT LEVEL", [this]()
    {
        manager.change(
            std::make_unique<PlayScene>(
                manager,
                window,
                level + 1
            )
        );
    });

    makeItem("MAIN MENU", [this]()
    {
        manager.change(
            std::make_unique<MainMenuScene>(
                manager,
                window
            )
        );
    });
}


// ================= LAYOUT =================

void LevelCompleteScene::initLayout()
{
    const float cx = window.getSize().x * 0.5f;

    if (titleText)
    {
        auto b = titleText->getLocalBounds();
        titleText->setOrigin({
            b.position.x + b.size.x * 0.5f,
            b.position.y + b.size.y * 0.5f
        });
        titleText->setPosition({ cx, 140.f });
    }

    if (coinText)
    {
        auto b = coinText->getLocalBounds();
        coinText->setOrigin({
            b.position.x + b.size.x * 0.5f,
            b.position.y + b.size.y * 0.5f
        });
        coinText->setPosition({ cx, 220.f });
    }

    float btnY = 360.f;
    float gap  = 130.f;

    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];
        if (!item.sprite || !item.text) continue;

        auto& btn = *item.sprite;
        auto& txt = *item.text;

        auto bb = btn.getLocalBounds();
        btn.setOrigin({
            bb.position.x + bb.size.x * 0.5f,
            bb.position.y + bb.size.y * 0.5f
        });
        btn.setPosition({ cx, btnY + i * gap });

        auto tb = txt.getLocalBounds();
        txt.setOrigin({
            tb.position.x + tb.size.x * 0.5f,
            tb.position.y + tb.size.y * 0.5f
        });
        txt.setPosition(btn.getPosition());
    }

    updateVisual();
}

void LevelCompleteScene::updateVisual()
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

void LevelCompleteScene::handleEvent(const sf::Event& e)
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

void LevelCompleteScene::update(float) {}

void LevelCompleteScene::render(sf::RenderWindow& window)
{
    if (bgSprite)
        window.draw(*bgSprite);

    if (titleText) window.draw(*titleText);
    if (coinText)  window.draw(*coinText);

    for (auto& item : items)
    {
        window.draw(*item.sprite);
        window.draw(*item.text);
    }
}

void LevelCompleteScene::handleMouseMove(sf::Vector2f mousePos)
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

void LevelCompleteScene::handleMouseClick(sf::Vector2f mousePos)
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