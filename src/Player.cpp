#include "../include/Player.hpp"
#include <algorithm> 
#include <iostream>

Player::Player() {
    body.setSize({50.f, 50.f});
    body.setFillColor(sf::Color::Green);
    body.setOrigin(body.getSize() / 2.f);
    body.setPosition({100.f, 100.f});

    velocity = {0.f, 0.f};
}

void Player::update(float dt) {
    handleInput(dt);
    applyGravity(dt);

    // clamp tốc độ rơi
    if (velocity.y > maxFallSpeed) {
        velocity.y = maxFallSpeed;
    }

    // cập nhật vị trí
    body.move(velocity * dt);
}

void Player::handleInput(float dt) {
    // trái phải
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)
    || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        velocity.x = -speed;
    } 
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        velocity.x = speed;
    }
    else {
        velocity.x = 0.f; 
    }

    // nhảy
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)
        || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        if(!spaceHeld) {
            jump();
            spaceHeld = true;
        }
        else {
            spaceHeld=false;
        }
    }
}

void Player::applyGravity(float dt) {
    velocity.y += gravity * dt;
}

void Player::move(const sf::Vector2f& movement) {
    body.move(movement);
}

void Player::correctPosition(const sf::Vector2f& correction) {
    body.move(correction);
    // correction.y < 0 nghĩa là player được đẩy lên → chạm đất
    if (correction.y < 0.f) {
        velocity.y = 0.f;
        canJump = true;
    }
    // correction.y > 0 nghĩa là đập đầu lên trần
    else if (correction.y > 0.f) {
        velocity.y = 0.f;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(body);
}

void Player::jump() {
    if (canJump) {
        velocity.y = -jumpHeight;
        canJump = false;
    }
}

sf::Rect<float> Player::getBounds() const {
    return sf::Rect<float>{ body.getPosition(), body.getSize() };
}

sf::Vector2f Player::getPosition() const {
    return body.getPosition();
}

void Player::setPosition(const sf::Vector2f& pos) {
    body.setPosition(pos);
}

void Player::hitTrap() {
    if (!justHitTrap && isAlive()) {
        health--;
        justHitTrap = true;
        trapTimer = 0.f; // reset timer
        std::cout << "Hit a trap! Health: " << health << std::endl;
        if (health <= 0) {
            setDead();
            std::cout << "Player is dead!" << std::endl;
        }
    }
}

void Player::updateTrapTimer(float dt) {
   if (justHitTrap) {
        trapTimer += dt;
        if (trapTimer >= trapCooldown) {
            justHitTrap = false;
            trapTimer = 0.f;
        }
    }
}
