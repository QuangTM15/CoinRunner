#include "../include/Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <cmath>

Player::Player()
    : texIdle()
    , texRun()
    , texJump()
    , sprite(texIdle)
{
    // --- LOAD TEXTURE (check lỗi 1 lần thôi) ---
    if (!texIdle.loadFromFile("asset/textures/player/Player_Idle.png"))
        std::cout << "[ERR] Cannot load idle texture\n";

    if (!texRun.loadFromFile("asset/textures/player/Player_Run.png"))
        std::cout << "[ERR] Cannot load run texture\n";

    if (!texJump.loadFromFile("asset/textures/player/Player_Jump.png"))
        std::cout << "[ERR] Cannot load jump texture\n";

    sprite.setTexture(texIdle);

    // mỗi frame trong spritesheet là 32x32
    // scale 2 → hiển thị 64x64, nhưng toạ độ vẫn tính theo pixel map (16x16 per tile)
    sprite.setScale({scale, scale});
    sprite.setOrigin({16.f, 16.f}); // center của frame 32x32

    // --- PHYSICS ---
speed        = 48.f;      // 3 tiles/s
gravity      = 1400.f;    // rơi nhanh, không bồng bềnh
desiredJumpHeight = 80.f; // 5 tiles

maxFallSpeed = 900.f;     // tốc độ rơi tối đa





    velocity     = {0.f, 0.f};
    trapCooldown = 1.0f;

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

// collider 20x28, nằm giữa frame 32x32
sf::Rect<float> Player::getBounds() const
{
    sf::Vector2f pos = sprite.getPosition();
    return {
        { pos.x - 8.f, pos.y - 14.f },   // dịch hitbox lên đúng
        { 16.f, 28.f }
    };
}


void Player::update(float dt)
{
    handleInput(dt);

    if (!isOnLadder)
        applyGravity(dt);
    else
        velocity.y = 0.f;

    if (velocity.y > maxFallSpeed)
        velocity.y = maxFallSpeed;

    updateTrapTimer(dt);
    updateAnimation(dt);
}


void Player::handleInput(float dt)
{
    float dir = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        dir = -1.f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        dir = 1.f;

    velocity.x = dir * speed;

    // flip sprite
    if (dir < 0) facingLeft = true;
    if (dir > 0) facingLeft = false;

    // Jump
    bool wantJump =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);

    if (wantJump && canJump)
    {
        float jumpVelocity = std::sqrt(2.f * gravity * desiredJumpHeight);
        velocity.y = -jumpVelocity;
        canJump = false;
    }


    // Ladder climb
    if (isOnLadder && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        velocity.y = -120.f; // tốc độ trèo
    }
}

void Player::applyGravity(float dt)
{
    velocity.y += gravity * dt;
}

void Player::correctPosition(const sf::Vector2f& correction)
{
    sprite.move(correction);

    if (correction.y < 0.f) // đẩy từ trên xuống → đứng trên đất
    {
        velocity.y = 0.f;
        canJump = true;
    }
    else if (correction.y > 0.f) // va trần
    {
        velocity.y = 0.f;
    }

    if (correction.x != 0.f)
    {
        velocity.x = 0.f;
    }
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);

    // debug: dot tại tâm
    sf::CircleShape dot(3.f);
    dot.setFillColor(sf::Color::Red);
    dot.setPosition(sprite.getPosition());
    window.draw(dot);
}

// ----------------------
//   ANIMATION SYSTEM
// ----------------------

void Player::setState(State newState)
{
    if (state == newState) return;
    state = newState;
    frame = 0;
    frameTimer = 0.f;
}

void Player::updateAnimation(float dt)
{
    // chọn state
    if (!canJump && velocity.y < 0.f)
        state = State::Jump;
    else if (!canJump && velocity.y > 0.f)
        state = State::Fall;
    else if (std::abs(velocity.x) > 1.f)
        state = State::Run;
    else
        state = State::Idle;

    // flip logic
    if (velocity.x < 0) facingLeft = true;
    if (velocity.x > 0) facingLeft = false;

    // Chọn texture + frame count
    const sf::Texture* tex = nullptr;
    int   maxFrames = 1;
    float fps       = 8.f;

    switch (state)
    {
        case State::Idle:
            tex       = &texIdle;
            maxFrames = idleFrames;
            fps       = idleFPS;
            break;

        case State::Run:
            tex       = &texRun;
            maxFrames = runFrames;
            fps       = runFPS;
            break;

        case State::Jump:
        case State::Fall:
            tex       = &texJump;
            maxFrames = jumpFrames;
            fps       = 8.f;
            break;
    }

    sprite.setTexture(*tex);

    const int frameW = 32;
    const int frameH = 32;

    // cập nhật frame
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

    // -----------------------------
    //   SET TEXTURE RECT (SFML 3)
    // -----------------------------
    sf::IntRect rect;

    if (!facingLeft)
    {
        // Hướng phải
        rect.position = { frame * frameW, 0 };
        rect.size     = { frameW, frameH };
    }
    else
    {
        // Hướng trái → mirror bằng size.x âm
        rect.position = { (frame + 1) * frameW, 0 };
        rect.size     = { -frameW, frameH };
    }

    sprite.setTextureRect(rect);
}

// ----------------------
//   DAMAGE & LIFE
// ----------------------

void Player::hitTrapDamage()
{
    if (!justHitTrap)
    {
        hp--;
        justHitTrap = true;
        trapTimer = 0.f;

        if (hp <= 0)
        {
            life--;
            if (life <= 0)
                alive = false;
            hp = maxHP;
        }
    }
}

void Player::hitSpider()
{
    hp = 0;
    life--;
    if (life <= 0)
        alive = false;
}

void Player::updateTrapTimer(float dt)
{
    if (justHitTrap)
    {
        trapTimer += dt;
        if (trapTimer >= trapCooldown)
        {
            justHitTrap = false;
            trapTimer   = 0.f;
        }
    }
}

void Player::respawn(const sf::Vector2f& pos)
{
    sprite.setPosition(pos);
    velocity = {0.f, 0.f};
    hp       = maxHP;
    alive    = true;
}

void Player::applyMovement(const sf::Vector2f& move, const sf::Vector2f& fix)
{
    // 1) Move theo input thực
    sprite.move(move);

    // 2) Sau đó mới sửa lệch collision
    sprite.move(fix);

    // 3) Reset velocity đúng cách
    if (fix.x != 0.f)
        velocity.x = 0.f;

    if (fix.y != 0.f)
    {
        velocity.y = 0.f;
        if (fix.y < 0) canJump = true; // đứng đất
    }
}