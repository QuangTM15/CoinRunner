#pragma once
#include <SFML/Graphics.hpp>
#include "Config.hpp"

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
    void applyMovement(const sf::Vector2f& move, const sf::Vector2f& fix);

    void draw(sf::RenderWindow& window);

    // Death system
    void onHitTrap();              // chạm trap = chết ngay
    void respawn(const sf::Vector2f& pos);

    bool isAlive() const { return alive; }
    bool facingLeft = false;

    int life = 7;
    float controlLock = 0.f;               

private:

    // --- Animation ---
    enum class State { Idle, Run, Jump, Fall };
    State state = State::Idle;

    sf::Texture texIdle;
    sf::Texture texRun;
    sf::Texture texJump;

    sf::Sprite sprite;

    float scale = 1.0f;

    int frame = 0;
    float frameTimer = 0.f;

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
    float desiredJumpHeight;
    float gravity;
    float maxFallSpeed;

    bool canJump = true;

    bool alive = true;
};