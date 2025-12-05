#pragma once
#include <SFML/Graphics.hpp>

enum class TrapType {
    Damage,   // trap_damage → trừ 1 máu
    Spider    // spider → chết luôn + trừ 1 mạng
};

class Trap {
private:
    sf::RectangleShape shape;
    TrapType type;

    bool playerInsideLastFrame = false;  // để xử lý "bước ra - bước vào"

public:
    Trap(float x, float y, float w, float h, TrapType trapType);

    void draw(sf::RenderWindow& window);

    // Kiểm tra va chạm, trả về:
    // 0 = không hit
    // 1 = trap_damage hit
    // 2 = spider hit
    int checkHit(const sf::FloatRect& playerBounds);

    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }

    // Load texture
    void setTexture(sf::Texture* tex);
};
