// #include "../include/Player.hpp"
// #include <algorithm> 
// #include <iostream>

// Player::Player() {
//     body.setSize({Config::TILE_SIZE, Config::TILE_SIZE});
//     body.setFillColor(sf::Color::Green);
//     body.setOrigin(body.getSize() / 2.f);
//     body.setPosition({100.f, 100.f});

//     speed = Config::PLAYER_SPEED;
//     jumpHeight = Config::PLAYER_JUMP_HEIGHT;
//     gravity = Config::PLAYER_GRAVITY;
//     maxFallSpeed = Config::PLAYER_MAX_FALL_SPEED;
//     health = Config::PLAYER_HEALTH;
//     trapCooldown = Config::TRAP_COOLDOWN;

//     velocity = {0.f, 0.f};
// }

// void Player::update(float dt) {
//     handleInput(dt);
//     applyGravity(dt);

//     // clamp tốc độ rơi
//     if (velocity.y > maxFallSpeed) {
//         velocity.y = maxFallSpeed;
//     }

//     // cập nhật vị trí
//     body.move(velocity * dt);
// }

// void Player::handleInput(float dt) {
//     // trái phải
//     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)
//     || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
//         velocity.x = -speed;
//     } 
//     else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)
//         || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
//         velocity.x = speed;
//     }
//     else {
//         velocity.x = 0.f; 
//     }
//     // nhảy
//     bool wantJump = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) ||
//                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
//                     sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
//     if (wantJump && !spaceHeld) {
//         jump();
//         spaceHeld = true;
//     }

//     if (!wantJump) {
//         spaceHeld = false;
//     }

// }

// void Player::applyGravity(float dt) {
//     velocity.y += gravity * dt;
// }

// void Player::move(const sf::Vector2f& movement) {
//     body.move(movement);
// }

// void Player::correctPosition(const sf::Vector2f& correction) {
//     body.move(correction);
//     // correction.y < 0 nghĩa là player được đẩy lên → chạm đất
//     if (correction.y < 0.f) {
//         velocity.y = 0.f;
//         canJump = true;
//     }
//     // correction.y > 0 nghĩa là đập đầu lên trần
//     else if (correction.y > 0.f) {
//         velocity.y = 0.f;
//     }
// }

// void Player::draw(sf::RenderWindow& window) {
//     window.draw(body);
// }

// void Player::jump() {
//     if (canJump) {
//         velocity.y = -jumpHeight;
//         canJump = false;
//     }
// }

// sf::Rect<float> Player::getBounds() const {
//     sf::Vector2f pos = body.getPosition();
//     sf::Vector2f size = body.getSize();

//     sf::Rect<float> temp;
//     temp.position.x   = pos.x - size.x / 2.f;
//     temp.position.y   = pos.y - size.y / 2.f;
//     temp.size.x  = size.x;
//     temp.size.y = size.y;

//     return temp;
// }


// sf::Vector2f Player::getPosition() const {
//     return body.getPosition();
// }

// void Player::setPosition(const sf::Vector2f& pos) {
//     body.setPosition(pos);
// }

// void Player::hitTrap() {
//     if (!justHitTrap && isAlive()) {
//         health--;
//         justHitTrap = true;
//         trapTimer = 0.f; // reset timer
//         std::cout << "Hit a trap! Health: " << health << std::endl;
//         if (health <= 0) {
//             setDead();
//             std::cout << "Player is dead!" << std::endl;
//         }
//     }
// }

// void Player::updateTrapTimer(float dt) {
//    if (justHitTrap) {
//         trapTimer += dt;
//         if (trapTimer >= trapCooldown) {
//             justHitTrap = false;
//             trapTimer = 0.f;
//         }
//     }
// }
