#include "ui/HUD.hpp"
#include <iostream>

HUD::HUD()
{
    // =================================================
    // FONT (SFML 3 dùng openFromFile)
    // =================================================
    if (!font.openFromFile("asset/ui/fonts/pixel.ttf"))
    {
        std::cerr << "[ERR] HUD font load FAILED\n";
        return;
    }

    // =================================================
    // ICON TEXTURES (member -> sống đủ lâu)
    // =================================================
    if (!texHeart.loadFromFile("asset/ui/icons/heart.png"))
        std::cerr << "[ERR] Cannot load heart.png\n";

    if (!texCoin.loadFromFile("asset/ui/icons/coin.png"))
        std::cerr << "[ERR] Cannot load coin.png\n";

    // =================================================
    // SPRITES (SFML 3 -> emplace, KHÔNG default ctor)
    // =================================================
    iconHeart.emplace(texHeart);
    iconCoin.emplace(texCoin);

    iconHeart->setPosition({12.f, 12.f});
    iconHeart->setScale({1.5f, 1.5f});
    iconCoin->setPosition({12.f, 52.f});
    iconCoin->setScale({1.5f, 1.5f});

    // =================================================
    // TEXT (SFML 3 -> emplace với font)
    // =================================================
    txtLife.emplace(font, "7", 16);
    txtCoin.emplace(font, "0", 16);

    txtLife->setPosition({52.f, 16.f});
    txtLife->setScale({1.5f, 1.5f});
    txtCoin->setPosition({52.f, 56.f});
    txtCoin->setScale({1.5f, 1.5f});

    txtLife->setFillColor(sf::Color::Red);
    txtCoin->setFillColor(sf::Color::Yellow);
}

// =================================================
// UPDATE LIFE
// =================================================
void HUD::setLife(int life)
{
    if (txtLife)
        txtLife->setString(std::to_string(life));
}

// =================================================
// UPDATE COIN
// =================================================
void HUD::setCoin(int coin)
{
    if (txtCoin)
        txtCoin->setString(std::to_string(coin));
}

// =================================================
// DRAW HUD (FIXED SCREEN – KHÔNG THEO CAMERA)
// =================================================
void HUD::draw(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());

    if (iconHeart) window.draw(*iconHeart);
    if (txtLife)   window.draw(*txtLife);

    if (iconCoin)  window.draw(*iconCoin);
    if (txtCoin)   window.draw(*txtCoin);
}