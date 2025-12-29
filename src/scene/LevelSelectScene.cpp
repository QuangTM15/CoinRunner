#include "scene/LevelSelectScene.hpp"
#include "scene/SceneManager.hpp"
#include <system/SaveManager.hpp>
#include <iostream>

LevelSelectScene::LevelSelectScene(SceneManager& mgr, sf::RenderWindow& win)
: Scene(mgr)
, window(win)
{}

void LevelSelectScene::onEnter()
{
    std::cout << "[LevelSelectScene] onEnter\n";

    maxUnlockedLevel = SaveManager::get().getMaxUnlockedLevel();

    if (!font.openFromFile("asset/ui/fonts/pixel.ttf"))
        std::cout << "[LevelSelect] Font load FAILED\n";

    levelTexts.clear();
    levelTexts.reserve(3); // tránh realloc

    // tạo text cho các level
    for (int i = 1; i <= 3; ++i)
    {
        std::string label = "LEVEL " + std::to_string(i);
        if (i > maxUnlockedLevel) label += " (LOCKED)";

        levelTexts.emplace_back(font, label, 16);
        auto& txt = levelTexts.back();
        txt.setScale({2.f, 2.f});

        // màu
        if (i <= maxUnlockedLevel) txt.setFillColor(sf::Color::White);
        else txt.setFillColor(sf::Color(120, 120, 120));

        // position (tạm để dễ test, chưa cần center origin)
        txt.setPosition({
            window.getSize().x * 0.5f - 120.f,
            200.f + (i - 1) * 80.f
        });
    }

    // tạo text cho nút Back// ---- BACK BUTTON ----
    backText.emplace(font, "BACK", 16);
    backText->setScale({2.f, 2.f});
    backText->setFillColor(sf::Color::White);

    // center origin (đúng rule)
    auto b = backText->getLocalBounds();
    backText->setOrigin({
        b.position.x + b.size.x * 0.5f,
        b.position.y + b.size.y * 0.5f
    });

    backText->setPosition({
        window.getSize().x * 0.5f,
        window.getSize().y - 120.f
    });
}

void LevelSelectScene::onExit()
{
    std::cout << "[LevelSelectScene] onExit\n";
}

void LevelSelectScene::handleEvent(const sf::Event&)
{
}

void LevelSelectScene::update(float)
{
}

void LevelSelectScene::render(sf::RenderWindow& window)
{
    for (auto& t : levelTexts)
        window.draw(t);
    if (backText) window.draw(*backText);
}