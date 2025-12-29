#include "scene/PlayScene.hpp"
#include "scene/SceneManager.hpp"
#include <iostream>
#include "system/AudioManager.hpp"
#include "system/SaveManager.hpp"

PlayScene::PlayScene(SceneManager& mgr, sf::RenderWindow& window)
: Scene(mgr)
, game(window.getSize().x, window.getSize().y)
{
    game.bindWindow(window);
}

void PlayScene::handleEvent(const sf::Event&)
{
    game.processEvents();
}

void PlayScene::update(float dt)
{
    game.update(dt);
    if (game.isLevelCompleted())
    {
        int nextLevel = game.getCurrentLevel() + 1;
        SaveManager::get().unlockLevel(nextLevel);
    }
}

void PlayScene::render(sf::RenderWindow&)
{
    static bool once = false;
    if (!once)
    {
        std::cout << "[PlayScene] render()\n";
        once = true;
    }
    game.render();
}

void PlayScene::onEnter()
{
    AudioManager::get().playBackground();
}

void PlayScene::onExit()
{
    AudioManager::get().stopBackground();
}