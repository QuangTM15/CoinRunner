#pragma once
#include <SFML/Graphics.hpp>

class SceneManager;

class Scene {
public:
    explicit Scene(SceneManager& mgr) : manager(mgr) {}
    virtual ~Scene() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void handleEvent(const sf::Event& e) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    

protected:
    SceneManager& manager;
};