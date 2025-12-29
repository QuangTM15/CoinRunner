#include "scene/LevelSelectScene.hpp"
#include "scene/SceneManager.hpp"
#include "scene/MainMenuScene.hpp"
#include "scene/PlayScene.hpp"
#include "system/SaveManager.hpp"

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
    if (!font.openFromFile("asset/ui/fonts/pixel.ttf"))
        std::cout << "[LevelSelect] Font load FAILED\n";
}

void LevelSelectScene::initLevelItems()
{
    items.clear();
    items.reserve(3);

    for (int i = 1; i <= 3; ++i)
    {
        bool unlocked = (i <= maxUnlockedLevel);

        std::string label = "LEVEL " + std::to_string(i);
        if (!unlocked) label += " (LOCKED)";

        sf::Text text(font, label, 16);
        text.setScale({2.f, 2.f});

        items.push_back(LevelItem{
            i,
            unlocked,
            std::move(text)
        });
    }

    // BACK
    backText.emplace(font, "BACK", 16);
    backText->setScale({2.f, 2.f});
}

void LevelSelectScene::initLayout()
{
    float cx = window.getSize().x * 0.5f;
    float startY = 200.f;
    float gap = 80.f;

    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& txt = items[i].text;
        auto b = txt.getLocalBounds();
        txt.setOrigin({
            b.position.x + b.size.x * 0.5f,
            b.position.y + b.size.y * 0.5f
        });
        txt.setPosition({cx, startY + i * gap});
    }

    if (backText)
    {
        auto b = backText->getLocalBounds();
        backText->setOrigin({
            b.position.x + b.size.x * 0.5f,
            b.position.y + b.size.y * 0.5f
        });
        backText->setPosition(
            {cx,
            window.getSize().y - 120.f
        });
    }
}

void LevelSelectScene::updateVisual()
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        auto& item = items[i];
        if (!item.unlocked)
        {
            item.text.setFillColor(sf::Color(120, 120, 120));
        }
        else if (i == selectedIndex)
        {
            item.text.setFillColor(sf::Color(255, 220, 40));
        }
        else
        {
            item.text.setFillColor(sf::Color::White);
        }
    }

    if (backText)
    {
        if (selectedIndex == (int)items.size())
            backText->setFillColor(sf::Color(255, 220, 40));
        else
            backText->setFillColor(sf::Color::White);
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

    if (key == sf::Keyboard::Key::Up)
    {
        selectedIndex = (selectedIndex - 1 + total) % total;
    }
    else if (key == sf::Keyboard::Key::Down)
    {
        selectedIndex = (selectedIndex + 1) % total;
    }
    else if (key == sf::Keyboard::Key::Enter)
    {
        if (selectedIndex == (int)items.size())
        {
            manager.change(
                std::make_unique<MainMenuScene>(manager, window)
            );
            return;
        }

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

    updateVisual();
}

void LevelSelectScene::handleMouseMove(sf::Vector2f pos)
{
    for (int i = 0; i < (int)items.size(); ++i)
    {
        if (items[i].text.getGlobalBounds().contains(pos))
        {
            selectedIndex = i;
            updateVisual();
            return;
        }
    }

    if (backText && backText->getGlobalBounds().contains(pos))
    {
        selectedIndex = items.size();
        updateVisual();
    }
}

void LevelSelectScene::handleMouseClick(sf::Vector2f pos)
{
    // BACK
    if (backText && backText->getGlobalBounds().contains(pos))
    {
        manager.change(
            std::make_unique<MainMenuScene>(manager, window)
        );
        return;
    }

    // LEVEL
    for (auto& item : items)
    {
        if (item.text.getGlobalBounds().contains(pos))
        {
            if (item.unlocked)
            {
                manager.change(
                    std::make_unique<PlayScene>(manager, window, item.level)
                );
            }
            return;
        }
    }
}

// ================= UPDATE / RENDER =================

void LevelSelectScene::update(float) {}

void LevelSelectScene::render(sf::RenderWindow& window)
{
    for (auto& item : items)
        window.draw(item.text);

    if (backText)
        window.draw(*backText);
}