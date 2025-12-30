#include "scene/SceneManager.hpp"
#include <iostream>

void SceneManager::change(std::unique_ptr<Scene> scene)
{
    while (!stack.empty())
    {
        stack.back()->onExit();
        stack.pop_back();
    }

    stack.push_back(std::move(scene));
    stack.back()->onEnter();
}

void SceneManager::push(std::unique_ptr<Scene> scene)
{
    stack.push_back(std::move(scene));
    stack.back()->onEnter();
}

void SceneManager::pop()
{
    if (stack.empty())
        return;

    stack.back()->onExit();
    stack.pop_back();
}

void SceneManager::handleEvent(const sf::Event& e)
{
    if (!stack.empty())
        stack.back()->handleEvent(e);
}

void SceneManager::update(float dt)
{
    if (!stack.empty())
        stack.back()->update(dt);
}

void SceneManager::render(sf::RenderWindow& window)
{
    if (stack.empty())
    {
        std::cout << "[SceneManager] render(): EMPTY STACK\n";
        return;
    }

    // 🔥 Render từ dưới lên (để Pause overlay lên Play)
    for (auto& s : stack)
        s->render(window);
}