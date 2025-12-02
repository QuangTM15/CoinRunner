#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Coin.hpp"
#include "Trap.hpp"

class TileMap {
private:
    std::vector<std::string> mapLayout; // text map
    float tileSize = 50.f;
    sf::RectangleShape tileShape;

public:
    std::vector<Coin> coins;
    std::vector<Trap> traps;

    TileMap();

    void loadDemoMap(); // load map text, tạo tile / coin / trap
    void draw(sf::RenderWindow& window);
    
    // Kiểm tra collision với player, trả correction vector
    sf::Vector2f checkCollision(const sf::Rect<float>& playerBounds, const sf::Vector2f& playerVelocity);
};
