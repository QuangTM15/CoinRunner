#pragma once
#include "Scene.hpp"
#include "../core/Game.hpp"

class PlayScene : public Scene {
public:
    PlayScene(SceneManager& mgr, sf::RenderWindow& window, int startLevel = 1);

    void handleEvent(const sf::Event& e) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;
    void onExit() override;

private:
    Game game;
};