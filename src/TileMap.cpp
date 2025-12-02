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

// Kiểm tra va chạm với player (với ground)
sf::Vector2f TileMap::checkCollision(const sf::Rect<float>& playerBounds, const sf::Vector2f& playerVelocity) {
    sf::Vector2f correction{0.f, 0.f};

    for (size_t y = 0; y < mapLayout.size(); y++) {
        for (size_t x = 0; x < mapLayout[y].size(); x++) {
            if (mapLayout[y][x] != '#') continue;

            // tạo tile Rect
            sf::Rect<float> tileRect;
            tileRect.position = { x * tileSize, y * tileSize };
            tileRect.size = { tileSize, tileSize };

            // AABB collision
            bool intersectX = playerBounds.position.x < tileRect.position.x + tileRect.size.x &&
                              playerBounds.position.x + playerBounds.size.x > tileRect.position.x;
            bool intersectY = playerBounds.position.y < tileRect.position.y + tileRect.size.y &&
                              playerBounds.position.y + playerBounds.size.y > tileRect.position.y;

            if (intersectX && intersectY) {
                // rơi xuống (player đang đi xuống)
                if (playerVelocity.y > 0.f) {
                    float overlapY = (playerBounds.position.y + playerBounds.size.y) - tileRect.position.y;
                    if (overlapY > 0.f && overlapY > correction.y) {
                        correction.y = -overlapY;   // đẩy player lên
                    }
                }
                // nhảy lên (player đang đi lên)
                else if (playerVelocity.y < 0.f) {
                    float overlapY = (tileRect.position.y + tileRect.size.y) - playerBounds.position.y;
                    if (overlapY > 0.f && -overlapY < correction.y) {
                        correction.y = overlapY;    // đẩy player xuống
                    }
                }
            }
        }
    }

    return correction;
}