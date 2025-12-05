#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct TilesetInfo {
    int firstgid;
    int columns;
    std::string imagePath;
    sf::Texture texture;
};
