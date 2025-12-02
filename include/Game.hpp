#pragma once
#include <SFML/Graphics.hpp>
#include "../include/Player.hpp"
#include "../include/TileMap.hpp"
#include <vector>

class Game {
public:
    Game(unsigned int windowWidth, unsigned int windowHeight);
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

private:
    sf::RenderWindow window;
    sf::Clock clock;

    Player player;
    TileMap tileMap;

    sf::View camera;

    int coinCount = 0;
};