#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Scene.hpp"

class SceneManager {
public:
    void change(std::unique_ptr<Scene> scene);
    void push(std::unique_ptr<Scene> scene);
    void pop();

    void handleEvent(const sf::Event& e);
    void update(float dt);
    void render(sf::RenderWindow& window);

    bool empty() const { return stack.empty(); }

private:
    std::vector<std::unique_ptr<Scene>> stack;
};