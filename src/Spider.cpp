#include "../include/Spider.hpp"
#include <cmath>

Spider::Spider(const sf::Texture& tex, const sf::Rect<float>& rect,
               float range, float speed)
    : moveRange(range), moveSpeed(speed), sprite(tex)
{
    sprite.setTexture(tex);

    sprite.setPosition(rect.position + rect.size * 0.5f);
    sprite.setOrigin(rect.size * 0.5f);

    startPos = sprite.getPosition();

    sprite.setScale({
        rect.size.x / tex.getSize().x,
        rect.size.y / tex.getSize().y
    });
}

void Spider::update(float dt)
{
    sprite.move({moveSpeed * direction * dt, 0});

    float dist = sprite.getPosition().x - startPos.x;

    if (std::abs(dist) >= moveRange)
    {
        direction *= -1.f;

        // flip sprite
        sprite.setScale({
            sprite.getScale().x * -1.f,
            sprite.getScale().y
        });
    }
}

void Spider::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

bool Spider::checkHit(const sf::Rect<float>& playerBounds)
{
    sf::Rect<float> s = sprite.getGlobalBounds();

    return (
        playerBounds.position.x < s.position.x + s.size.x &&
        playerBounds.position.x + playerBounds.size.x > s.position.x &&
        playerBounds.position.y < s.position.y + s.size.y &&
        playerBounds.position.y + playerBounds.size.y > s.position.y
    );
}