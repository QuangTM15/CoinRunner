#include "core/Trap.hpp"
#include <cmath>

Trap::Trap(Type t,
           const sf::Texture& tex,
           const sf::Vector2f& pos)
    : type(t)
    , texture(&tex)
    , sprite(tex)
{
    // set origin giữa sprite
    sf::Vector2u size = tex.getSize();
    sprite.setOrigin({size.x * 0.5f, size.y * 0.5f});

    sprite.setPosition(pos);
    startPos = pos;
}

void Trap::update(float dt)
{
    if (type == Type::Static)
        return;

    timeCounter += dt * speed;
    float offset = std::sin(timeCounter) * range;

    if (moveAxisX)
        sprite.setPosition({startPos.x + offset, startPos.y});
    else
        sprite.setPosition({startPos.x, startPos.y + offset});
}

void Trap::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}