#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class HUD {
public:
    HUD();

    void setLife(int life);
    void setCoin(int coin);
    void draw(sf::RenderWindow& window);

private:
    sf::Font font;

    sf::Texture texHeart;
    sf::Texture texCoin;

    std::optional<sf::Sprite> iconHeart;
    std::optional<sf::Sprite> iconCoin;

    std::optional<sf::Text> txtLife;
    std::optional<sf::Text> txtCoin;
};