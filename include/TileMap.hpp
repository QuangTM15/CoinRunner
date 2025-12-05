#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "TileSetInfo.hpp"

class TileMap : public sf::Drawable {
public:
    TileMap() = default;
    bool loadFromFile(const std::string& mapFile, float tileSize);

private:
    float mapWidth = 0, mapHeight = 0;
    float tileSize = 0;

    std::vector<TilesetInfo> tilesets;
    std::vector<sf::Sprite> tiles;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};