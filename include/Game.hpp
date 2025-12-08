#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Player.hpp"
#include "TileMap.hpp"
#include "Coin.hpp"
#include "DamageTrap.hpp"
#include "Spider.hpp"

class Game {
public:
    Game(unsigned int windowWidth, unsigned int windowHeight);
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void loadObjectsFromMap();

    void updateCoins(float dt);
    void updateTraps(float dt);
    void updateSpiders(float dt);
    void updateCamera();

private:
    sf::RenderWindow window;
    sf::Clock clock;
    sf::View camera;

    float camWidth;
    float camHeight;

    // -----------------------
    //  GAME ENTITIES
    // -----------------------
    Player player;
    TileMap tileMap;

    std::vector<Coin> coins;
    std::vector<DamageTrap> traps;
    std::vector<Spider> spiders;

    int coinCount = 0;
    sf::Vector2f lastCheckpoint;

    // -----------------------
    //  TEXTURES (Persistent!)
    // -----------------------
    sf::Texture texCoin;
    sf::Texture texTrapDamage;
    sf::Texture texSpider;
};