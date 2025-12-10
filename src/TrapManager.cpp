#include "TrapManager.hpp"
#include <cmath>


sf::IntRect TrapManager::gidToRect(int gid, const TileMap& map) const
{
    const TilesetInfo& ts = map.tilesets[0]; // vì bạn chỉ có 1 tileset 16x16
    int local = gid - ts.firstgid;

    int tx = (local % ts.columns) * map.tileWidth;
    int ty = (local / ts.columns) * map.tileHeight;
    sf::IntRect r;
    r.position = { tx, ty };
    r.size     = { map.tileWidth, map.tileHeight };
    return r;

}

void TrapManager::loadFromTileMap(const TileMap& map)
{
    traps.clear();

    // STATIC TRAPS (st)
    for (auto& obj : map.trapsStatic)
    {
        sf::Vector2f center = obj.rect.position + sf::Vector2f(8.f, 8.f);

        int gid = obj.gid & 0x1FFFFFFF;
        sf::IntRect rect = gidToRect(gid, map);

        traps.emplace_back(Trap::Type::Static,
                           map.tilesets[0].texture,
                           center,
                           rect);
    }

    // MOVING TRAPS (mv)
    for (auto& obj : map.trapsMoving)
    {
        sf::Vector2f center = obj.rect.position + sf::Vector2f(8.f, 8.f);

        int gid = obj.gid & 0x1FFFFFFF;
        sf::IntRect rect = gidToRect(gid, map);

        Trap t(Trap::Type::Moving,
               map.tilesets[0].texture,
               center,
               rect);

        float range = obj.floatProps.count("range") ?
                      obj.floatProps.at("range") : 40.f;

        bool axisX = obj.floatProps.count("axis") ?
                     (obj.floatProps.at("axis") == 0.f) : true;

        t.setRange(range);
        t.setAxisX(axisX);
        t.setSpeed(2.0f);

        traps.push_back(t);
    }
}

void TrapManager::update(float dt)
{
    for (auto& t : traps)
        t.update(dt);
}

void TrapManager::draw(sf::RenderWindow& window)
{
    for (auto& t : traps)
        t.draw(window);
}

bool TrapManager::checkPlayerCollision(Player& player) const
{
    for (auto& t : traps)
    {
        if (intersects(player.getBounds(), t.getBounds()))
            return true;
    }
    return false;
}