#pragma once
#include <SFML/Graphics.hpp>

class Coin {
public:
    Coin(const sf::Texture& tex, const sf::Vector2f& pos, int frames = 12, float fps = 12.f);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool checkCollected(const sf::Rect<float>& playerBounds);

    bool isCollected() const { return collected; }

    sf::Rect<float> getBounds() const;

    static int getTotalCollected() { return totalCollected; }

private:

    const sf::Texture* texture = nullptr;
    sf::Sprite sprite;

    int frameCount;
    int currentFrame = 0;

    float frameTimer = 0.f;
    float frameSpeed;

    bool collected = false;

    static int totalCollected;
};
