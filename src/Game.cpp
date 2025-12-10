#include "Game.hpp"
#include <iostream>

// ------------------------------------------------
// Helper intersects for SFML3
// ------------------------------------------------
static bool intersectsRect(const sf::Rect<float>& a, const sf::Rect<float>& b)
{
    return !(
        a.position.x + a.size.x < b.position.x ||
        a.position.x > b.position.x + b.size.x ||
        a.position.y + a.size.y < b.position.y ||
        a.position.y > b.position.y + b.size.y
    );
}

// ------------------------------------------------
// Constructor
// ------------------------------------------------
Game::Game(unsigned int windowWidth, unsigned int windowHeight)
{
    window.create(sf::VideoMode({windowWidth, windowHeight}), "Coin Runner Game");

    camWidth  = windowWidth;
    camHeight = windowHeight;

    camera.setSize({ camWidth, camHeight });

    // -------------------------
    // LOAD TEXTURES
    // -------------------------
    if (!texCoin.loadFromFile("asset/textures/items/Coin.png"))
        std::cerr << "[ERR] Cannot load Coin.png\n";

    // Tileset dùng cho trap st/mv
    if (!texTileset.loadFromFile("asset/textures/items/16x16.png"))
        std::cerr << "[ERR] Cannot load 16x16 tileset!\n";

    // -------------------------
    // LOAD MAP
    // -------------------------
    tileMap.loadFromFile("asset/maps/mapdemo.json", 16.f);

    loadObjectsFromMap();

    // Spawn player
    player.setPosition(tileMap.spawnPoint);
    lastCheckpoint = tileMap.spawnPoint;

    camera.setCenter(player.getPosition());
    camera.zoom(0.5f);
    window.setView(camera);
}

// ------------------------------------------------
// Load trap/coins/checkpoint from TileMap
// ------------------------------------------------
void Game::loadObjectsFromMap()
{
    // -------- COINS --------
    for (auto& obj : tileMap.coins)
    {
        coins.emplace_back(
            texCoin,
            obj.rect.position,
            12.f, 12.f
        );
    }

    // -------- STATIC TRAPS (st) --------
    for (auto& obj : tileMap.trapsStatic)
    {
        int gid = obj.gid & 0x1FFFFFFF;
        int local = gid - tileMap.tilesets[0].firstgid;

        int tx = (local % tileMap.tilesets[0].columns) * tileMap.tileWidth;
        int ty = (local / tileMap.tilesets[0].columns) * tileMap.tileHeight;

        sf::IntRect rect;
        rect.position = { tx, ty };
        rect.size = { tileMap.tileWidth, tileMap.tileHeight };

        sf::Vector2f center = obj.rect.position + sf::Vector2f(8.f, 8.f);

        traps.emplace_back(Trap::Type::Static, texTileset, center, rect);
    }

    // -------- MOVING TRAPS (mv) --------
    for (auto& obj : tileMap.trapsMoving)
    {
        int gid = obj.gid & 0x1FFFFFFF;
        int local = gid - tileMap.tilesets[0].firstgid;

        int tx = (local % tileMap.tilesets[0].columns) * tileMap.tileWidth;
        int ty = (local / tileMap.tilesets[0].columns) * tileMap.tileHeight;

        sf::IntRect rect;
        rect.position = { tx, ty };
        rect.size = { tileMap.tileWidth, tileMap.tileHeight };

        sf::Vector2f center = obj.rect.position + sf::Vector2f(8.f, 8.f);

        Trap t(Trap::Type::Moving, texTileset, center, rect);

        t.setRange(obj.floatProps.count("range") ? obj.floatProps.at("range") : 40.f);
        t.setAxisX(!obj.floatProps.count("axis") || obj.floatProps.at("axis") == 0);

        traps.push_back(t);
    }
}


// ------------------------------------------------
// UPDATE
// ------------------------------------------------
void Game::update(float dt)
{
    player.update(dt);

    sf::Vector2f move = player.getVelocity() * dt;
    sf::Vector2f fix  = tileMap.checkCollision(player.getBounds(), move);

    player.applyMovement(move, fix);

    sf::Rect<float> pbox = player.getBounds();

    // ---- UPDATE COINS ----
    updateCoins(dt);

    // ---- UPDATE TRAPS ----
    updateTraps(dt);

    // ---- KILLZONES ----
    for (auto& r : tileMap.killzones)
    {
        if (intersectsRect(pbox, r))
        {
            player.onHitTrap();
            player.respawn(lastCheckpoint);
        }
    }

    // ---- CHECKPOINT ----
    for (auto& cp : tileMap.checkpoints)
    {
        if (intersectsRect(pbox, cp.rect))
            lastCheckpoint = cp.rect.position;
    }

    updateCamera();
}


// ------------------------------------------------
// COINS
// ------------------------------------------------
void Game::updateCoins(float dt)
{
    for (auto it = coins.begin(); it != coins.end();)
    {
        it->update(dt);

        if (it->checkCollected(player.getBounds()))
        {
            coinCount++;
            it = coins.erase(it);
        }
        else ++it;
    }
}


// ------------------------------------------------
// TRAPS (st + mv)
// ------------------------------------------------
void Game::updateTraps(float dt)
{
    for (auto& t : traps)
    {
        t.update(dt);

        if (intersectsRect(player.getBounds(), t.getBounds()))
        {
            player.onHitTrap();
            player.respawn(lastCheckpoint);
        }
    }
}


// ------------------------------------------------
// CAMERA FOLLOW
// ------------------------------------------------
void Game::updateCamera()
{
    sf::Vector2f center = player.getPosition();

    float mapW = tileMap.getmapWidth()  * tileMap.tileWidth;
    float mapH = tileMap.getmapHeight() * tileMap.tileHeight;

    float halfW = camera.getSize().x * 0.5f;
    float halfH = camera.getSize().y * 0.5f;

    auto clamp = [&](float v, float lo, float hi)
    {
        return std::max(lo, std::min(hi, v));
    };

    center.x = clamp(center.x, halfW, mapW - halfW);
    center.y = clamp(center.y, halfH, mapH - halfH);

    camera.setCenter(center);
}


// ------------------------------------------------
// RENDER
// ------------------------------------------------
void Game::render()
{
    window.clear();
    window.setView(camera);

    window.draw(tileMap);

    for (auto& c : coins) c.draw(window);
    for (auto& t : traps) t.draw(window);

    player.draw(window);

    window.display();
}


// ------------------------------------------------
// GAME LOOP
// ------------------------------------------------
void Game::run()
{
    sf::Clock clock;
    float acc = 0.f;
    const float dtFixed = 1.f / 60.f;

    while (window.isOpen())
    {
        processEvents();

        float dt = clock.restart().asSeconds();
        acc += dt;

        while (acc >= dtFixed)
        {
            update(dtFixed);
            acc -= dtFixed;
        }

        render();
    }
}


// ------------------------------------------------
// PROCESS EVENTS
// ------------------------------------------------
void Game::processEvents()
{
    while (auto evt = window.pollEvent())
    {
        if (evt->is<sf::Event::Closed>())
            window.close();
    }
}