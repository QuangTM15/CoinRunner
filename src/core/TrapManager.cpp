#include "core/TrapManager.hpp"

void TrapManager::setTextures(const sf::Texture& st,
                              const sf::Texture& mvX,
                              const sf::Texture& mvY)
{
    texStatic = &st;
    texMoveX  = &mvX;
    texMoveY  = &mvY;
}

void TrapManager::loadFromTileMap(const TileMap& map)
{
    traps.clear();

    if (!texStatic || !texMoveX || !texMoveY)
        return;

    // -------- STATIC TRAPS --------
    for (auto& obj : map.trapsStatic)
    {
        sf::Vector2f center = obj.rect.position + obj.rect.size * 0.5f;

        traps.emplace_back(
            Trap::Type::Static,
            *texStatic,
            center
        );
    }

    // -------- MOVING TRAPS --------
    for (auto& obj : map.trapsMoving)
    {
        bool axisX = !obj.floatProps.count("axis") ||
                     obj.floatProps.at("axis") == 0.f;

        float range = obj.floatProps.count("range") ?
                      obj.floatProps.at("range") : 40.f;

        sf::Vector2f center = obj.rect.position + obj.rect.size * 0.5f;

        Trap t(
            Trap::Type::Moving,
            axisX ? *texMoveX : *texMoveY,
            center
        );

        t.setAxisX(axisX);
        t.setRange(range);
        t.setSpeed(2.f);

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