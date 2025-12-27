#pragma once
#include <memory>
#include "Scene.hpp"

class SceneManager {
public:
    void change(std::unique_ptr<Scene> scene);

    void handleEvent(const sf::Event& e);
    void update(float dt);
    void render(sf::RenderWindow& window);

private:
    std::unique_ptr<Scene> current;
};
