#pragma once
#include <SFML/Graphics.hpp>

class Coin {
private:
    sf::CircleShape shape;
    bool collected = false;
    static int totalCollected;

public:
    Coin(float x, float y);

    void draw(sf::RenderWindow& window);

    // Trả true nếu player chạm và thu coin
    bool checkCollected(const sf::FloatRect& playerBounds);

    bool isCollected() const { return collected; }

    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    static int getTotalCollected() { return totalCollected; }
};
