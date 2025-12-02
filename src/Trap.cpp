#include "../include/Trap.hpp"

Trap::Trap(float x, float y, float width, float height) {
    shape.setSize({width, height});
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin({width/2.f, height/2.f});
    shape.setPosition({x, y});
}

void Trap::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Trap::checkHit(const sf::Rect<float>& playerBounds) {
    sf::Rect<float> trapBounds = shape.getGlobalBounds();

    // AABB collision check
    if (playerBounds.position.x < trapBounds.position.x + trapBounds.size.x &&
        playerBounds.position.x + playerBounds.size.x > trapBounds.position.x &&
        playerBounds.position.y < trapBounds.position.y + trapBounds.size.y &&
        playerBounds.position.y + playerBounds.size.y > trapBounds.position.y)
    {
        return true; // player chạm trap
    }
    return false;
}
