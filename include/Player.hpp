#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "config.hpp"

class Player {
    private:
        sf::RectangleShape body;
        sf::Vector2f velocity;
    
        float speed;
        float jumpHeight;
        float gravity;
        float maxFallSpeed;

        bool canJump = true;
        bool spaceHeld = false;
        bool alive = true;
        bool justHitTrap = false;  // tránh trừ máu liên tục khi chạm trap
        float trapCooldown; // thời gian delay giữa các lần trúng trap (giây)
        float trapTimer = 0.f;      // bộ đếm

    public:
        Player();
        void update(float dt);
        void handleInput(float dt);
        void applyGravity(float dt);
        void move(const sf::Vector2f& movement);
        void correctPosition(const sf::Vector2f& correction);

        void draw(sf::RenderWindow& window);

        sf::Rect<float> getBounds() const;
        sf::Vector2f getPosition() const;
        void setPosition(const sf::Vector2f& pos);
        void jump();
        int health = 100;
        bool isAlive() const { return alive; }
        void setDead() { alive = false; }
        sf::Vector2f getVelocity() const { return velocity; }
        void hitTrap();
        void updateTrapTimer(float dt);
};