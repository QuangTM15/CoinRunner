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

    // truyền texture trap từ Game vào
    void setTextures(const sf::Texture& st,
                     const sf::Texture& mvX,
                     const sf::Texture& mvY);

    void loadFromTileMap(const TileMap& map);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool checkPlayerCollision(Player& player) const;

private:
    std::vector<Trap> traps;

    // textures KHÔNG SỞ HỮU (lifetime do Game quản lý)
    const sf::Texture* texStatic = nullptr;
    const sf::Texture* texMoveX  = nullptr;
    const sf::Texture* texMoveY  = nullptr;
};