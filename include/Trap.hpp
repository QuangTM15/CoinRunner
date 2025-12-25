#pragma once
#include <SFML/Graphics.hpp>

class Trap
{
public:
    enum class Type { Static, Moving };

    Trap(Type type,
         const sf::Texture& texture,
         const sf::Vector2f& pos);

    // config moving trap
    void setRange(float r)    { range = r; }
    void setAxisX(bool x)     { moveAxisX = x; }
    void setSpeed(float s)    { speed = s; }

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

private:
    Type type;

    const sf::Texture* texture = nullptr;
    sf::Sprite sprite;

    // moving behaviour
    float range     = 0.f;
    float speed     = 1.f;
    bool  moveAxisX = true;

    sf::Vector2f startPos;
    float timeCounter = 0.f;
};

