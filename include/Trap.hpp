#pragma once
#include <SFML/Graphics.hpp>

class Trap {
private:
    sf::RectangleShape shape;

public:
    Trap(float x, float y, float width, float height);

    void draw(sf::RenderWindow& window);

    // Trả true nếu player chạm trap
    bool checkHit(const sf::Rect<float>& playerBounds);

    sf::Rect<float> getBounds() const { return shape.getGlobalBounds(); }
};
