#include "../include/DamageTrap.hpp"

DamageTrap::DamageTrap(const sf::Texture& tex, const sf::Rect<float>& rect,
                       float damage, float interval)
    : damageAmount(damage), damageInterval(interval),sprite(tex)
{
    sprite.setTexture(tex);
    sprite.setPosition(rect.position + rect.size * 0.5f);
    sprite.setOrigin(rect.size * 0.5f);

    sprite.setScale({
        rect.size.x / tex.getSize().x,
        rect.size.y / tex.getSize().y
    });
}

void DamageTrap::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

bool DamageTrap::checkHit(const sf::Rect<float>& playerBounds, float dt)
{
    sf::Rect<float> t = sprite.getGlobalBounds();

    bool inside =
        playerBounds.position.x < t.position.x + t.size.x &&
        playerBounds.position.x + playerBounds.size.x > t.position.x &&
        playerBounds.position.y < t.position.y + t.size.y &&
        playerBounds.position.y + playerBounds.size.y > t.position.y;

    if (!inside)
    {
        playerInside = false;
        timer = 0.f;
        return false;
    }

    // Nếu vừa bước vào trap → reset timer ngay
    if (!playerInside)
    {
        playerInside = true;
        timer = damageInterval; // gây damage ngay lập tức
    }

    // Tăng timer
    timer += dt;

    if (timer >= damageInterval)
    {
        timer = 0.f;
        return true;    // GÂY DAMAGE
    }

    return false;
}
