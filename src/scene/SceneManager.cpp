#include "scene/SceneManager.hpp"
#include <iostream>

void SceneManager::change(std::unique_ptr<Scene> scene)
{
    if (current)
        current->onExit();

    current = std::move(scene);

    if (current)
        current->onEnter();
}

void SceneManager::handleEvent(const sf::Event& e)
{
    if (current)
        current->handleEvent(e);
}

void SceneManager::update(float dt)
{
    if (current)
        current->update(dt);
}

void SceneManager::render(sf::RenderWindow& window)
{
    if (!current)
    {
        std::cout << "[SceneManager] render(): NO CURRENT SCENE\n";
        return;
    }

    current->render(window);
}