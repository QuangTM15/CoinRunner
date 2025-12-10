#pragma once
#include <SFML/Graphics.hpp>

class Trap
{
public:
    enum class Type { Static, Moving };

    // tex  : texture tileset (16x16.png)
    // pos  : vị trí tâm trap (từ Tiled object)
    // rect : ô tile trên spritesheet
    Trap(Type type,
         const sf::Texture& tex,
         const sf::Vector2f& pos,
         const sf::IntRect& texRect);

    // config cho moving trap
    void setRange(float r)      { range = r; }
    void setAxisX(bool xAxis)   { moveAxisX = xAxis; }
    void setSpeed(float s)      { speed = s; }

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f  getPosition() const { return sprite.getPosition(); }

private:
    Type type;

    const sf::Texture* texture = nullptr;
    sf::Sprite         sprite;
    sf::IntRect        tileRect;

    // moving behaviour
    float range      = 0.f;   // biên độ dao động (pixel)
    float speed      = 1.f;  // tốc độ dao động
    bool  moveAxisX  = true;  // true = X, false = Y

    sf::Vector2f startPos;
    float        timeCounter = 0.f;
};
