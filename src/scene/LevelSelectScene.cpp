#include "scene/LevelSelectScene.hpp"
#include "scene/SceneManager.hpp"
#include <iostream>

LevelSelectScene::LevelSelectScene(SceneManager& mgr, sf::RenderWindow& win)
: Scene(mgr)
, window(win)
{}

void LevelSelectScene::onEnter()
{
    std::cout << "[LevelSelectScene] onEnter\n";
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

void LevelSelectScene::render(sf::RenderWindow&)
{
    // tạm thời chưa vẽ gì
}