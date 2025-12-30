#include "core/Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <cmath>
#include "core/Config.hpp"

Player::Player()
    : texIdle(), texRun(), texJump(), sprite(texIdle)
{
    if (!texIdle.loadFromFile("asset/textures/player/Player_Idle.png"))
        std::cout << "[ERR] Cannot load idle texture\n";

    if (!texRun.loadFromFile("asset/textures/player/Player_Run.png"))
        std::cout << "[ERR] Cannot load run texture\n";

    if (!texJump.loadFromFile("asset/textures/player/Player_Jump.png"))
        std::cout << "[ERR] Cannot load jump texture\n";

    sprite.setTexture(texIdle);
    sprite.setScale({scale, scale});
    sprite.setOrigin({16.f, 16.f});

    // physics
    speed        = Config::PLAYER_SPEED;
    gravity      = Config::PLAYER_GRAVITY;
    desiredJumpHeight = Config::PLAYER_JUMP_HEIGHT;
    maxFallSpeed = Config::PLAYER_MAX_FALL_SPEED;

    velocity = {0.f, 0.f};

    setPosition({100.f, 100.f});
    updateAnimation(0.f);
}

void Player::setPosition(const sf::Vector2f& pos)
{
    sprite.setPosition(pos);
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}

sf::Rect<float> Player::getBounds() const
{
    sf::Vector2f pos = sprite.getPosition();

    return {
        { pos.x - 8.f, pos.y - 14.f },
        { 16.f, 28.f }
    };
}

void Player::update(float dt)
{
    if (controlLock > 0.f)
    {
        controlLock -= dt;  
    }
    else
    {
        handleInput(dt);
    }

    applyGravity(dt);
    if (velocity.y > maxFallSpeed)
        velocity.y = maxFallSpeed;
    updateAnimation(dt);

}

void Player::handleInput(float dt)
{
    float dir = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dir = -1.f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dir = 1.f;

    velocity.x = dir * speed;

    if (dir < 0) facingLeft = true;
    if (dir > 0) facingLeft = false;

    bool wantJump =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);

    if (wantJump && canJump)
    {
        float jumpVelocity = std::sqrt(2.f * gravity * desiredJumpHeight);
        velocity.y = -jumpVelocity;
        canJump = false;
        jumpedThisFrame = true; 
    }
}

void Player::applyGravity(float dt)
{
    velocity.y += gravity * dt;
}

void Player::correctPosition(const sf::Vector2f& correction)
{
    sprite.move(correction);

    if (correction.y < 0.f)
    {
        velocity.y = 0.f;
        canJump = true;
    }
    else if (correction.y > 0.f)
    {
        velocity.y = 0.f;
    }

    if (correction.x != 0.f)
        velocity.x = 0.f;
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Player::setState(State newState)
{
    if (state == newState) return;
    state = newState;
    frame = 0;
    frameTimer = 0.f;
}

void Player::updateAnimation(float dt)
{
    if (!canJump && velocity.y < 0.f)
        state = State::Jump;
    else if (!canJump && velocity.y > 0.f)
        state = State::Fall;
    else if (std::abs(velocity.x) > 1.f)
        state = State::Run;
    else
        state = State::Idle;

    const sf::Texture* tex = nullptr;
    int   maxFrames = 1;
    float fps       = 8.f;

    switch (state)
    {
        case State::Idle:
            tex = &texIdle;
            maxFrames = idleFrames;
            fps = idleFPS;
            break;

        case State::Run:
            tex = &texRun;
            maxFrames = runFrames;
            fps = runFPS;
            break;

        case State::Jump:
        case State::Fall:
            tex = &texJump;
            maxFrames = jumpFrames;
            break;
    }

    sprite.setTexture(*tex);

    const int frameW = 32;
    const int frameH = 32;

    frameTimer += dt;
    if (frameTimer >= 1.f / fps)
    {
        frameTimer = 0.f;
        frame++;

        if (frame >= maxFrames)
        {
            if (state == State::Jump || state == State::Fall)
                frame = maxFrames - 1;
            else
                frame = 0;
        }
    }

    sf::IntRect rect;

    if (!facingLeft)
    {
        rect.position = { frame * frameW, 0 };
        rect.size = { frameW, frameH };
    }
    else
    {
        rect.position = { (frame + 1) * frameW, 0 };
        rect.size = { -frameW, frameH };
    }

    sprite.setTextureRect(rect);
}

// ----------------------
//   Death / Respawn
// ----------------------

void Player::onHitTrap()
{
    controlLock = 0.2f;
}

void Player::respawn(const sf::Vector2f& pos)
{
    sprite.setPosition(pos);
    velocity = {0.f, 0.f};
    canJump = true;
    controlLock = 0.1f;
}

void Player::applyMovement(const sf::Vector2f& move, const sf::Vector2f& fix)
{
    // ===== X AXIS FIRST =====
    sprite.move({ move.x + fix.x, 0.f });

    if (fix.x != 0.f)
        velocity.x = 0.f;

    // ===== Y AXIS SECOND =====
    sprite.move({ 0.f, move.y + fix.y });

    if (fix.y != 0.f)
    {
        velocity.y = 0.f;

        if (fix.y < 0)
            canJump = true;  // landed
    }
}
