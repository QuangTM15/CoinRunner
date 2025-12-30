#pragma once
#include <memory>
#include <vector>
#include "Scene.hpp"

class SceneManager {
public:
    void change(std::unique_ptr<Scene> scene);
    void push(std::unique_ptr<Scene> scene);
    void pop();

    void handleEvent(const sf::Event& e);
    void update(float dt);
    void render(sf::RenderWindow& window);

private:
    std::vector<std::unique_ptr<Scene>> stack;
};