#include "scene/SceneManager.hpp"

// ---------------- CHANGE ----------------
void SceneManager::change(std::unique_ptr<Scene> scene)
{
    // exit toàn bộ scene hiện tại
    while (!stack.empty())
    {
        stack.back()->onExit();
        stack.pop_back();
    }

    stack.push_back(std::move(scene));
    stack.back()->onEnter();
}

// ---------------- PUSH ----------------
void SceneManager::push(std::unique_ptr<Scene> scene)
{
    stack.push_back(std::move(scene));
    stack.back()->onEnter();
}

// ---------------- POP ----------------
void SceneManager::pop()
{
    if (stack.empty())
        return;

    stack.back()->onExit();
    stack.pop_back();
}

// ---------------- EVENT ----------------
void SceneManager::handleEvent(const sf::Event& e)
{
    if (!stack.empty())
        stack.back()->handleEvent(e);
}

// ---------------- UPDATE ----------------
void SceneManager::update(float dt)
{
    if (!stack.empty())
        stack.back()->update(dt);
}

// ---------------- RENDER ----------------
void SceneManager::render(sf::RenderWindow& window)
{
    for (auto& s : stack)
        s->render(window);
}