#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Trap.hpp"
#include "TileMap.hpp"
#include "Player.hpp"
static bool intersects(const sf::Rect<float>& a, const sf::Rect<float>& b)
{
    return !(
        a.position.x + a.size.x < b.position.x ||
        a.position.x > b.position.x + b.size.x ||
        a.position.y + a.size.y < b.position.y ||
        a.position.y > b.position.y + b.size.y
    );
}

class TrapManager {
public:
    TrapManager() = default;

    // Load trap từ tilemap
    void loadFromTileMap(const TileMap& map);

    // update + draw
    void update(float dt);
    void draw(sf::RenderWindow& window);

    // player chạm trap?
    bool checkPlayerCollision(Player& player) const;

private:
    std::vector<Trap> traps;

    // Helper để chuyển GID → IntRect
    sf::IntRect gidToRect(int gid, const TileMap& map) const;
};