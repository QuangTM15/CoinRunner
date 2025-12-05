#pragma once
#include <SFML/Graphics.hpp>

class DamageTrap
{
public:
    DamageTrap(const sf::Texture& tex, const sf::Rect<float>& rect,
               float damage = 1.f, float interval = 1.f);

    void draw(sf::RenderWindow& window);

    // Trả về true nếu gây damage (cứ mỗi interval)
    bool checkHit(const sf::Rect<float>& playerBounds, float dt);

    float getDamage() const { return damageAmount; }

private:
    sf::Sprite sprite;

    float damageAmount;   // số damage mỗi lần tick
    float damageInterval; // thời gian delay giữa 2 lần gây damage
    float timer = 0.f;    // đếm thời gian

    bool playerInside = false;
};
