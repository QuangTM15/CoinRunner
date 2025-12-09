#pragma once
#include <SFML/Graphics.hpp>
#include "Config.hpp"
#include <unordered_map>
static constexpr int SPRITE_W = 32;
static constexpr int SPRITE_H = 32;

class Player {
public:
    Player();

    void update(float dt);
    void handleInput(float dt);
    void applyGravity(float dt);

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;

    sf::Rect<float> getBounds() const;
    sf::Vector2f getVelocity() const { return velocity; }

    void correctPosition(const sf::Vector2f& correction);

    void draw(sf::RenderWindow& window);

    // Trap xử lý
    void hitTrapDamage(); // nấm
    void hitSpider();     // nhện
    void updateTrapTimer(float dt);

    // Ladder
    void setOnLadder(bool v) { isOnLadder = v; }
    bool getOnLadder() const { return isOnLadder; }

    // HP + Life
    int hp = 5;
    int maxHP = 5;
    int life = 3;

    bool isAlive() const { return alive; }
    void respawn(const sf::Vector2f& pos);
    bool facingLeft = false;

private:

    // --- Animation ---
    enum class State { Idle, Run, Jump, Fall };
    State state = State::Idle;

    sf::Texture texIdle;
    sf::Texture texRun;
    sf::Texture texJump;

    sf::Sprite sprite;
    // body is only for collision detection
    float scale = 1.5f;

    int frame = 0;
    float frameTimer = 0.f;

    // số frame mỗi trạng thái
    int idleFrames = 4;
    int runFrames = 8;
    int jumpFrames = 6;

    float idleFPS = 6.f;
    float runFPS = 10.f;

    void updateAnimation(float dt);
    void setState(State newState);

private:
    // physics
    sf::Vector2f velocity;

    float speed;
    float jumpHeight;
    float gravity;
    float maxFallSpeed;

    bool spaceHeld = false;
    bool canJump = true;
    bool isOnLadder = false;

    bool alive = true;

    // trap damage
    bool justHitTrap = false;
    float trapCooldown;
    float trapTimer = 0.f;
};