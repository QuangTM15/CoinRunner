#include "Game.hpp"
#include <iostream>

// ------------------------------------------------
//  Constructor
// ------------------------------------------------

static bool intersects(const sf::Rect<float>& a, const sf::Rect<float>& b)
{
    return !(
        a.position.x + a.size.x < b.position.x ||
        a.position.x > b.position.x + b.size.x ||
        a.position.y + a.size.y < b.position.y ||
        a.position.y > b.position.y + b.size.y
    );
}

Game::Game(unsigned int windowWidth, unsigned int windowHeight)
{
    window.create(sf::VideoMode({windowWidth, windowHeight}), "Coin Runner Game");

    camWidth  = windowWidth;
    camHeight = windowHeight;

    camera.setSize({camWidth, camHeight});  
    camera.setCenter(tileMap.spawnPoint);
    camera.zoom(0.5f);
    window.setView(camera);



    // ---------------------------------
    // LOAD ALL TEXTURES (persistent!)
    // ---------------------------------
    if (!texCoin.loadFromFile("asset/textures/items/Coin.png"))
        std::cerr << "[ERR] Cannot load Coin texture\n";

    if (!texTrapDamage.loadFromFile("asset/textures/traps/damage.png"))
        std::cerr << "[ERR] Cannot load trap_damage texture\n";

    if (!texSpider.loadFromFile("asset/textures/enemies/spider.png"))
        std::cerr << "[ERR] Cannot load spider texture\n";

    // Load map
    tileMap.loadFromFile("asset/maps/mapdemo.json", 16.f);

    // Load all objects
    loadObjectsFromMap();

    // Player spawn
    player.setPosition(tileMap.spawnPoint);
    std::cout << "SPAWN = " << tileMap.spawnPoint.x << ", "<< tileMap.spawnPoint.y << "\n";

    lastCheckpoint = tileMap.spawnPoint;
}


// ------------------------------------------------
//  Load objects from TileMap
// ------------------------------------------------
void Game::loadObjectsFromMap()
{
    // Coins
    for (auto& obj : tileMap.coins)
    {
        coins.emplace_back(texCoin, obj.rect.position, 12.f, 12.f);
    }

    // Damage Traps
    for (auto& obj : tileMap.trapDamages)
    {
        float interval = obj.floatProps.count("interval") ? obj.floatProps.at("interval") : 1.f;

        traps.emplace_back(
            texTrapDamage,
            obj.rect,
            1.f,          // damage
            interval
        );
    }

    // Spiders
    for (auto& obj : tileMap.spiders)
    {
        float range = obj.floatProps.count("range") ? obj.floatProps.at("range") : 50.f;
        float speed = obj.floatProps.count("speed") ? obj.floatProps.at("speed") : 50.f;

        spiders.emplace_back(texSpider, obj.rect, range, speed);
    }
}


// ------------------------------------------------
//  Main Loop
// ------------------------------------------------
void Game::run()
{
    sf::Clock clock;
    float accumulator = 0.f;
    const float fixedDt = 1.f / 60.f;   // update logic 60 lần/giây

    while (window.isOpen())
    {
        processEvents();

        float dt = clock.restart().asSeconds();
        accumulator += dt;

        // Update cố định, không phụ thuộc FPS
        while (accumulator >= fixedDt)
        {
            update(fixedDt);       // 🔥 logic mượt tuyệt đối
            accumulator -= fixedDt;
        }

        render();                  // render theo FPS không cố định
    }
}


// ------------------------------------------------
//  Events
// ------------------------------------------------
void Game::processEvents()
{
    while (auto evt = window.pollEvent())
    {
        if (evt->is<sf::Event::Closed>())
            window.close();
    }
}


// ------------------------------------------------
//  UPDATE
// ------------------------------------------------
void Game::update(float dt)
{
    player.update(dt);

    sf::Vector2f vel = player.getVelocity();
    sf::Vector2f move = vel * dt;

    sf::Vector2f fix = tileMap.checkCollision(player.getBounds(), move);

    player.applyMovement(move, fix);

    // cập nhật pbox sau khi move
    sf::Rect<float> pbox = player.getBounds();

    // ladder
    bool onLadder = false;
    for (auto& r : tileMap.ladderAreas)
        if (intersects(pbox, r)) onLadder = true;
    player.setOnLadder(onLadder);

    // traps / coins / checkpoints...
    updateCoins(dt);
    updateTraps(dt);
    updateSpiders(dt);

    for (auto& r : tileMap.killzones)
        if (intersects(pbox, r))
            player.respawn(lastCheckpoint);

    updateCamera();
}

// ------------------------------------------------
//  COINS
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
//  TRAPS
// ------------------------------------------------
void Game::updateTraps(float dt)
{
    for (auto& t : traps)
    {
        if (t.checkHit(player.getBounds(), dt))
            player.hitTrapDamage();
    }
}


// ------------------------------------------------
//  SPIDERS
// ------------------------------------------------
void Game::updateSpiders(float dt)
{
    for (auto& s : spiders)
    {
        s.update(dt);

        if (s.checkHit(player.getBounds()))
        {
            player.hitSpider();
            player.respawn(lastCheckpoint);
        }
    }
}


// ------------------------------------------------
//  CAMERA
// ------------------------------------------------
void Game::updateCamera()
{
    sf::Vector2f center = player.getPosition();

    float mapPixelW = tileMap.getmapWidth()  * 16.f;
    float mapPixelH = tileMap.getmapHeight() * 16.f;

    float halfW = camera.getSize().x * 0.5f;
    float halfH = camera.getSize().y * 0.5f;

    float minX, maxX, minY, maxY;

    // --- Horizontal ---
    if (mapPixelW <= camera.getSize().x) {
        minX = maxX = mapPixelW * 0.5f;
    } else {
        minX = halfW;
        maxX = mapPixelW - halfW;
    }

    // --- Vertical ---
    if (mapPixelH <= camera.getSize().y) {
        minY = maxY = mapPixelH * 0.5f;
    } else {
        minY = halfH;
        maxY = mapPixelH - halfH;
    }

    auto clamp = [&](float v, float lo, float hi)
    {
        if (v < lo) return lo;
        if (v > hi) return hi;
        return v;
    };

    center.x = clamp(center.x, minX, maxX);
    center.y = clamp(center.y, minY, maxY);

    camera.setCenter(center);
}

void Game::render()
{
    window.clear(sf::Color::Black);
    window.setView(camera);
    window.draw(tileMap);

    for (auto& c : coins)   c.draw(window);
    for (auto& t : traps)   t.draw(window);
    for (auto& s : spiders) s.draw(window);

    player.draw(window);

    window.display();
}
