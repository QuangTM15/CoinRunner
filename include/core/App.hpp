#pragma once
#include <SFML/Graphics.hpp>
#include "scene/SceneManager.hpp"

class App {
public:
    App();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

private:
    sf::RenderWindow window;
    SceneManager sceneManager;
    sf::Clock clock;
};
