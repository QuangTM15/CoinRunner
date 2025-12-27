#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Player.hpp"
#include "TileMap.hpp"
#include "Coin.hpp"
#include "Trap.hpp"

class Game {
public:
    // ------------------------------------------------
    // Constructor
    Game(unsigned int windowWidth, unsigned int windowHeight);

    // 🔹 API
    void bindWindow(sf::RenderWindow& win);
    void processEvents();
    void update(float dt);
    void render();

private:

    void loadObjectsFromMap();
    void updateCoins(float dt);
    void updateTraps(float dt);
    void updateCamera();
    void loadLevel(int level);

private:
    sf::RenderWindow* window = nullptr;
    sf::View camera;

    float camWidth;
    float camHeight;

    Player player;
    TileMap tileMap;

    std::vector<Coin> coins;
    std::vector<Trap> traps;

    int coinCount = 0;
    sf::Vector2f lastCheckpoint;

    sf::Texture texCoin;
    sf::Texture texTrapStatic;
    sf::Texture texTrapMoveX;
    sf::Texture texTrapMoveY;
    int currentLevel = 1;
    bool reachedEndGame = false;
    bool touchedGoal = false;
};