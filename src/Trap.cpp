#include "Trap.hpp"
#include <cmath>

Trap::Trap(Type t,
           const sf::Texture& tex,
           const sf::Vector2f& pos,
           const sf::IntRect& texRect)
    : type(t)
    , texture(&tex)
    , sprite(tex)    
    , tileRect(texRect)
{
    // chọn đúng ô trên spritesheet
    sprite.setTextureRect(tileRect);

    // SFML 3: dùng size.x / size.y
    sprite.setOrigin({tileRect.size.x * 0.5f,tileRect.size.y * 0.5f});

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
        sprite.setPosition({ startPos.x + offset, startPos.y });
    else
        sprite.setPosition({ startPos.x, startPos.y + offset });
}

void Trap::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}