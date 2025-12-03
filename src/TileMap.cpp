#include "../include/TileMap.hpp"

// Demo map layout
TileMap::TileMap() {
    tileShape.setSize({tileSize, tileSize});
    tileShape.setFillColor(sf::Color(100,100,100));
    tileShape.setOutlineThickness(1.f);
    tileShape.setOutlineColor(sf::Color::Black);

    loadDemoMap();
}

void TileMap::loadDemoMap() {
    // '#' = ground, '$' = coin, '!' = trap, ' ' = empty
    mapLayout = {
        "###############################",
        "#                             #",
        "#   $       $         $       #",
        "#           !                 #",
        "#####           #####         #",
        "#       $                     #",
        "#   !        $       !        #",
        "#     #####         ####      #",
        "#       $                     #",
        "#   $       !        $        #",
        "#           #####             #",
        "#   $                     !   #",
        "#######           #######     #",
        "#       $       $             #",
        "#    !           !       $    #",
        "#     #######                  #",
        "#   $              $     !    #",
        "#          $                  #",
        "###############################"
    };
    coins.clear();
    traps.clear();
    for (size_t y = 0; y < mapLayout.size(); y++) {
        for (size_t x = 0; x < mapLayout[y].size(); x++) {
            char c = mapLayout[y][x];
            float px = x * tileSize + tileSize/2.f;
            float py = y * tileSize + tileSize/2.f;
            if (c == '$') coins.emplace_back(px, py);
            else if (c == '!') traps.emplace_back(px, py, tileSize, tileSize);
        }
    }
}

// Vẽ tile map + coin + trap
void TileMap::draw(sf::RenderWindow& window) {
    for (size_t y = 0; y < mapLayout.size(); y++) {
        for (size_t x = 0; x < mapLayout[y].size(); x++) {
            if (mapLayout[y][x] == '#') {
                tileShape.setPosition({x*tileSize, y*tileSize});
                window.draw(tileShape);
            }
        }
    }

    for (auto& coin : coins) coin.draw(window);
    for (auto& trap : traps) trap.draw(window);
}

sf::Vector2f TileMap::checkCollision(const sf::Rect<float>& playerBounds, const sf::Vector2f& playerVelocity) {
    sf::Vector2f totalCorrection{0.f, 0.f};

    for (size_t y = 0; y < mapLayout.size(); y++) {
        for (size_t x = 0; x < mapLayout[y].size(); x++) {
            if (mapLayout[y][x] != '#') continue;

            sf::Rect<float> tileRect;
            tileRect.position = { x * tileSize, y * tileSize };
            tileRect.size = { tileSize, tileSize };

            // AABB collision check
            bool intersectX = playerBounds.position.x + playerBounds.size.x > tileRect.position.x &&
                              playerBounds.position.x < tileRect.position.x + tileRect.size.x;
            bool intersectY = playerBounds.position.y + playerBounds.size.y > tileRect.position.y &&
                              playerBounds.position.y < tileRect.position.y + tileRect.size.y;

            if (!intersectX || !intersectY) continue;

            // Calculate overlap
            float overlapLeft   = (playerBounds.position.x + playerBounds.size.x) - tileRect.position.x;
            float overlapRight  = (tileRect.position.x + tileRect.size.x) - playerBounds.position.x;
            float overlapTop    = (playerBounds.position.y + playerBounds.size.y) - tileRect.position.y;
            float overlapBottom = (tileRect.position.y + tileRect.size.y) - playerBounds.position.y;

            // Choose smallest overlap for each axis
            float xCorrection = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
            float yCorrection = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;

            // Apply the axis with the smaller overlap first
            if (std::abs(xCorrection) < std::abs(yCorrection)) {
                totalCorrection.x += xCorrection;
            } else {
                totalCorrection.y += yCorrection;
            }
        }
    }

    return totalCorrection;
}
