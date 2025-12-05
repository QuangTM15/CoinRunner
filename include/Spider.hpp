#pragma once
#include <SFML/Graphics.hpp>

class Spider
{
public:
    Spider(const sf::Texture& tex, const sf::Rect<float>& rect,
           float range, float speed);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool checkHit(const sf::Rect<float>& playerBounds);

private:
    sf::Sprite sprite;

    sf::Vector2f startPos;
    float moveRange;
    float moveSpeed;

    float direction = 1.f;   // 1 = right, -1 = left
};
