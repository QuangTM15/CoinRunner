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
    bool isLevelCompleted() const { return levelCompleted; }
    bool isEndGame() const { return reachedEndGame; }
    int  getCurrentLevel() const { return currentLevel; }
    void startLevel(int level);

private:

    // 🔹 Internal methods
    void loadObjectsFromMap();
    void updateCoins(float dt);
    void updateTraps(float dt);
    void updateCamera();
    void loadLevel(int level);

private:

    sf::RenderWindow* window = nullptr;
    sf::View camera;
    sf::Vector2f cameraCenter;
    float camWidth;
    float camHeight;
    // Game entities
    Player player;
    TileMap tileMap;
    // Game objects
    std::vector<Coin> coins;
    std::vector<Trap> traps;


    int coinCount = 0;
    // Checkpoint
    sf::Vector2f lastCheckpoint;

    // Textures
    sf::Texture texCoin;
    sf::Texture texTrapStatic;
    sf::Texture texTrapMoveX;
    sf::Texture texTrapMoveY;

    // ---- Level management ----
    int currentLevel = 1;
    bool reachedEndGame = false;
    bool touchedGoal = false;
    bool levelCompleted = false;

    // ---- Camera shake ----
    bool  shakeActive = false;
    float shakeTime   = 0.f;
    float shakeTimer  = 0.f;
    float shakePower  = 0.f;
    void startCameraShake(float duration, float power);
};