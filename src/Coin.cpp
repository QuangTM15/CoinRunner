#include "../include/Coin.hpp"

int Coin::totalCollected = 0;

Coin::Coin(const sf::Texture& tex, const sf::Vector2f& pos, int frames, float fps)
    : texture(&tex), sprite(tex)
{
    texture = &tex;
    sprite.setTexture(tex);

    frameCount = frames;
    frameSpeed = fps;

    // Origin center
    sprite.setOrigin({8.f, 8.f});

    // Scale giống player
    sprite.setScale({2.f, 2.f});

    sprite.setPosition(pos);

    // Set frame đầu tiên
    sf::IntRect rect;
    rect.position = {0, 0};
    rect.size     = {16, 16};
    sprite.setTextureRect(rect);
}

void Coin::update(float dt)
{
    if (collected) return;

    frameTimer += dt;
    if (frameTimer >= 1.f / frameSpeed)
    {
        frameTimer = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;

        sf::IntRect rect;
        rect.position = {currentFrame * 16, 0};
        rect.size     = {16, 16};

        sprite.setTextureRect(rect);
    }
}

void Coin::draw(sf::RenderWindow& window)
{
    if (!collected)
        window.draw(sprite);
}

sf::Rect<float> Coin::getBounds() const
{
    auto gb = sprite.getGlobalBounds();
    return sf::Rect<float>(gb.position, gb.size);
}

bool Coin::checkCollected(const sf::Rect<float>& playerBounds)
{
    if (collected) return false;

    sf::Rect<float> coinBounds = getBounds();

    // AABB collision
    if (playerBounds.position.x < coinBounds.position.x + coinBounds.size.x &&
        playerBounds.position.x + playerBounds.size.x > coinBounds.position.x &&
        playerBounds.position.y < coinBounds.position.y + coinBounds.size.y &&
        playerBounds.position.y + playerBounds.size.y > coinBounds.position.y)
    {
        collected = true;
        totalCollected++;
        return true;
    }
    return false;
}