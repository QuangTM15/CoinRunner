#include "../include/Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Player::Player()
{
    // --- LOAD TEXTURE ---
    texIdle.loadFromFile("asset/textures/player/Player_Idle.png");
    texRun.loadFromFile("asset/textures/player/Player_Run.png");
    texJump.loadFromFile("asset/textures/player/Player_Jump.png");

    sprite.setTexture(texIdle);
    sprite.setScale({scale, scale});

    sprite.setOrigin({8, 8}); // frame 16x16 → origin center

    // --- PHYSICS ---
    speed = 120.f;
    jumpHeight = 250.f;
    gravity = 600.f;
    maxFallSpeed = 500.f;

    velocity = {0.f, 0.f};

    // trap damage timer
    trapCooldown = 1.0f;

    // default spawn (sẽ được set từ Game)
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
    float w = 16.f;
    float h = 16.f;

    return sf::Rect<float>(
        sf::Vector2f(pos.x - w/2, pos.y - h/2),
        sf::Vector2f(w, h)
    );
}

void Player::update(float dt)
{
    handleInput(dt);

    // gravity nếu không trèo thang
    if (!isOnLadder)
        applyGravity(dt);
    else
        velocity.y = 0.f;

    if (velocity.y > maxFallSpeed)
        velocity.y = maxFallSpeed;

    sprite.move(velocity * dt);

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
    if (dir < 0) sprite.setScale({-scale, scale});
    if (dir > 0) sprite.setScale({scale, scale});

    // Jump
    bool wantJump =
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);

    if (wantJump && canJump)
    {
        velocity.y = -jumpHeight;
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

    if (correction.y < 0) {
        velocity.y = 0.f;
        canJump = true;
    }
    else if (correction.y > 0) {
        velocity.y = 0.f;
    }
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
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
    if (!canJump && velocity.y < 0)
        state = State::Jump;
    else if (!canJump && velocity.y > 0)
        state = State::Fall;
    else if (std::abs(velocity.x) > 1.f)
        state = State::Run;
    else
        state = State::Idle;

    // Chọn texture + frame count
    const sf::Texture* tex = nullptr;
    int maxFrames = 1;
    float fps = 8.f;

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
            fps = 8.f;
            break;
    }

    sprite.setTexture(*tex);

    const int frameW = 16;
    const int frameH = 16;

    // cập nhật frame
    frameTimer += dt;
    if (frameTimer >= 1.f / fps)
    {
        frameTimer = 0.f;
        frame++;

        // Jump/Fall giữ frame cuối
        if (frame >= maxFrames)
            frame = (state == State::Jump || state == State::Fall)
                  ? maxFrames - 1
                  : 0;
    }

    // Pixel rectangle (SFML 3 dùng IntRect.position + size)
    sf::IntRect rect;
    rect.position = { frame * frameW, 0 };
    rect.size     = { frameW, frameH };

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
            trapTimer = 0.f;
        }
    }
}

void Player::respawn(const sf::Vector2f& pos)
{
    sprite.setPosition(pos);
    velocity = {0.f, 0.f};
    hp = maxHP;
    alive = true;
}