// #include "../include/Trap.hpp"

// Trap::Trap(float x, float y, float w, float h, TrapType trapType)
//     : type(trapType)
// {
//     shape.setSize({w, h});
//     shape.setOrigin({w/2.f, h/2.f});
//     shape.setPosition({x, y});
// }

// void Trap::setTexture(sf::Texture* tex) {
//     if (tex) {
//         shape.setTexture(tex);
//     }
// }

// void Trap::draw(sf::RenderWindow& window) {
//     window.draw(shape);
// }

// int Trap::checkHit(const sf::FloatRect& playerBounds)
// {
//     sf::FloatRect t = shape.getGlobalBounds();
//     bool isInside =
//         playerBounds.intersects(t);

//     if (!isInside) {
//         playerInsideLastFrame = false;
//         return 0;
//     }

//     // Player vừa bước vào trap (không phải đứng im trong trap)
//     if (!playerInsideLastFrame) {
//         playerInsideLastFrame = true;

//         if (type == TrapType::Damage) {
//             return 1; // trừ 1 máu
//         }
//         if (type == TrapType::Spider) {
//             return 2; // trừ hết máu + mất 1 mạng
//         }
//     }

//     // đang đứng trong trap nhưng không tính thêm damage
//     return 0;
// }
