#include "core/Game.hpp"
#include <iostream>
#include <algorithm>
#include "core/Config.hpp"
#include <cmath>

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
    camera.setSize({ Config::GAME_W, Config::GAME_H });

    // -------------------------
    // LOAD TEXTURES
    // -------------------------
    if (!texCoin.loadFromFile("asset/textures/items/Coin.png"))
        std::cerr << "[ERR] Cannot load Coin.png\n";

    //Load trap st/mv
    if (!texTrapStatic.loadFromFile("asset/textures/items/trap_st.png"))
        std::cerr << "[ERR] Cannot load trap_st.png\n";

    if (!texTrapMoveX.loadFromFile("asset/textures/items/trap_mv_x.png"))
        std::cerr << "[ERR] Cannot load trap_mv_x.png\n";

    if (!texTrapMoveY.loadFromFile("asset/textures/items/trap_mv_y.png"))
        std::cerr << "[ERR] Cannot load trap_mv_y.png\n";

    // -------------------------
    // LOAD MAP
    // -------------------------
    loadLevel(1);

    camera.setCenter(player.getPosition());
    camera.zoom(0.5f);
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
        sf::Vector2f center = obj.rect.position + obj.rect.size * 0.5f;

        traps.emplace_back(
            Trap::Type::Static,
            texTrapStatic,
            center
        );
    }

    // -------- MOVING TRAPS (mv) --------
    for (auto& obj : tileMap.trapsMoving)
    {
        bool axisX = !obj.floatProps.count("axis") || obj.floatProps.at("axis") == 0;
        float range = obj.floatProps.count("range") ? obj.floatProps.at("range") : 40.f;

        sf::Vector2f center = obj.rect.position + obj.rect.size * 0.5f;

        Trap t(
            Trap::Type::Moving,
            axisX ? texTrapMoveX : texTrapMoveY,
            center
        );

        t.setAxisX(axisX);
        t.setRange(range);

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

    updateCoins(dt);
    updateTraps(dt);

    // ---- CHECK KILLZONE 
    for (auto& r : tileMap.killzones)
    {
        if (intersectsRect(pbox, r))
        {
            player.onHitTrap();
            startCameraShake(0.25f, 5.f);
            player.respawn(lastCheckpoint);
        }
    }

    // ---- CHECK CHECKPOINT + GOAL ----
    for (auto& cp : tileMap.checkpoints)
    {
        if (intersectsRect(pbox, cp.rect))
            lastCheckpoint = cp.rect.position;
    }
    
    // Goal
    for (auto& g : tileMap.goals)
    {
        if (intersectsRect(pbox, g.rect))
        {
            if (!touchedGoal)
            {
                touchedGoal = true;

                if (currentLevel < 3)
                {
                    std::cout << "[LEVEL] COMPLETE -> LOAD LEVEL "
                              << (currentLevel + 1) << "\n";
                    loadLevel(currentLevel + 1);
                }
                else
                {
                    std::cout << "[END GAME] DEV DI HOC ROI :))\n";
                    reachedEndGame = true;
                }
            }
        }
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
            startCameraShake(0.2f, 4.f);
            player.respawn(lastCheckpoint);
        }
    }
}


// ------------------------------------------------
// CAMERA FOLLOW
// ------------------------------------------------
void Game::updateCamera()
{
    // ---- target theo player + clamp (như bạn đang có) ----
    sf::Vector2f target = player.getPosition();

    float mapW  = tileMap.getmapWidth()  * tileMap.tileWidth;
    float mapH  = tileMap.getmapHeight() * tileMap.tileHeight;

    float halfW = camera.getSize().x * 0.5f;
    float halfH = camera.getSize().y * 0.5f;

    if (target.x < halfW) target.x = halfW;
    else if (target.x > mapW - halfW) target.x = mapW - halfW;

    if (target.y < halfH) target.y = halfH;
    else if (target.y > mapH - halfH) target.y = mapH - halfH;

    // ---- LERP ----
    const float lerp = 0.1f;
    cameraCenter += (target - cameraCenter) * lerp;

    // ---- SHAKE ----
    sf::Vector2f finalCenter = cameraCenter;

    if (shakeActive)
    {
        shakeTimer -= 1.f / 60.f; // vì dt đã clamp

        float offsetX = (std::rand() / (float)RAND_MAX * 2.f - 1.f) * shakePower;
        float offsetY = (std::rand() / (float)RAND_MAX * 2.f - 1.f) * shakePower;

        finalCenter += { offsetX, offsetY };

        if (shakeTimer <= 0.f)
        {
            shakeActive = false;
        }
    }

    camera.setCenter(finalCenter);
}

// ------------------------------------------------
// RENDER
// ------------------------------------------------
void Game::render()
{
    if (!window)
        return;

    window->setView(camera);

    // MAP
    window->draw(tileMap);

    // COINS
    for (auto& c : coins)
        c.draw(*window);

    // TRAPS
    for (auto& t : traps)
        t.draw(*window);

    // PLAYER
    player.draw(*window);
}


// --------------------LOAD LEVEL----------------------
void Game::loadLevel(int level)
{
    currentLevel = level;

    std::string path = "asset/maps/level" + std::to_string(level) + ".json";
    std::cout << "[LOAD] " << path << "\n";

    tileMap.loadFromFile(path, Config::TILE_SIZE);

    coins.clear();
    traps.clear();
    loadObjectsFromMap();

    // --- reset player (BẮT BUỘC) ---
    sf::Vector2f spawn = tileMap.spawnPoint;
    spawn.y -= 2.f;                     // nhấc lên 2px tránh dính tile

    player.setPosition(spawn);
    lastCheckpoint = spawn;
    touchedGoal = false;

    camera.setCenter(player.getPosition());
    cameraCenter = camera.getCenter();
}

// ------------------------------------------------
// PROCESS EVENTS
// ------------------------------------------------
void Game::processEvents()
{
    while (auto evt = window->pollEvent())
    {
        if (evt->is<sf::Event::Closed>())
            window->close();
    }
}
void Game::bindWindow(sf::RenderWindow& win)
{
    window = &win;

    float winW = static_cast<float>(window->getSize().x);
    float winH = static_cast<float>(window->getSize().y);

    float scaleX = winW / Config::GAME_W;
    float scaleY = winH / Config::GAME_H;
    float scale  = std::floor(std::min(scaleX, scaleY));

    if (scale < 1.f) scale = 1.f;

    sf::Vector2f viewSize(
        Config::GAME_W / scale,
        Config::GAME_H / scale
    );

    camera.setSize(viewSize);
    camera.setCenter({viewSize.x * 0.5f, viewSize.y * 0.5f});

    window->setView(camera);
}
void Game::startCameraShake(float duration, float power)
{
    shakeActive = true;
    shakeTime   = duration;
    shakeTimer  = duration;
    shakePower  = power;
}