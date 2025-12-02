#include "../include/Coin.hpp"
int Coin::totalCollected = 0; // khởi tạo

Coin::Coin(float x, float y) {
    shape.setRadius(15.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin({shape.getRadius(), shape.getRadius()});
    shape.setPosition({x, y});
}

void Coin::draw(sf::RenderWindow& window) {
    if (!collected) {
        window.draw(shape);
    }
}

// Check va chạm với player theo AABB
bool Coin::checkCollected(const sf::Rect<float>& playerBounds) {
    if (collected) return false;

    sf::Rect<float> coinBounds = shape.getGlobalBounds();

    // AABB collision check SFML3
    if (playerBounds.position.x < coinBounds.position.x + coinBounds.size.x &&
        playerBounds.position.x + playerBounds.size.x > coinBounds.position.x &&
        playerBounds.position.y < coinBounds.position.y + coinBounds.size.y &&
        playerBounds.position.y + playerBounds.size.y > coinBounds.position.y)
    {
        collected = true;
        return true;
    }
    return false;
}