#include "scene/MainMenuScene.hpp"
#include <iostream>

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

void MainMenuScene::handleEvent(const sf::Event&)
{
    // TODO: bật input sau
}

void MainMenuScene::update(float)
{
}

void MainMenuScene::render(sf::RenderWindow& window)
{
    static bool once = false;
    if (!once)
    {
        std::cout << "[MainMenuScene] render()\n";
        once = true;
    }
    if (panelSprite)
        window.draw(*panelSprite);

    for (auto& item : items)
    {
        if (item.sprite)
            window.draw(*item.sprite);
        else if (item.text)
            window.draw(*item.text);
    }
}

// ================= Init =================

void MainMenuScene::initAssets()
{
    // uiFont.emplace();
    // uiFont->loadFromFile("asset/ui/fonts/pixel.ttf");

    // panelTexture.emplace();
    // panelTexture->loadFromFile("asset/ui/panels/panel.png");
    // panelSprite.emplace(*panelTexture);
}

void MainMenuScene::initMenuItems()
{
    // START
    MenuItem start;
    start.action = [](){};
    items.push_back(std::move(start));

    // LEVEL SELECT
    MenuItem level;
    level.action = [](){};
    items.push_back(std::move(level));

    // EXIT
    MenuItem exit;
    exit.action = [](){};
    items.push_back(std::move(exit));
}

void MainMenuScene::initLayout()
{
    // TODO
}

void MainMenuScene::handleKeyboard(sf::Keyboard::Key)
{
}

void MainMenuScene::handleMouseMove(sf::Vector2f)
{
}

void MainMenuScene::handleMouseClick(sf::Vector2f)
{
}

void MainMenuScene::updateVisual()
{
}
