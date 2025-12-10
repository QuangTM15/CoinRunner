#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include "TileSetInfo.hpp"

class TileMap : public sf::Drawable {
public:

    // ---- MAP OBJECT STRUCT ----
    struct MapObject {
        sf::Rect<float> rect;                        // vị trí và kích thước
        int gid = 0;                                  // tile GID (áp dụng cho coin, trap, checkpoint)
        std::unordered_map<std::string, float> floatProps; // ví dụ: range, axis
    };

    TileMap() = default;
    bool loadFromFile(const std::string& mapFile, float tileSize);

    // ---- WORLD DATA ----
    sf::Vector2f spawnPoint;

    std::vector<MapObject> coins;         // object "coin"
    std::vector<MapObject> trapsStatic;   // object "st"
    std::vector<MapObject> trapsMoving;   // object "mv"
    std::vector<MapObject> checkpoints;   // object "checkpoint"
    std::vector<sf::Rect<float>> killzones; // killzone rect (die on touch)

    // ---- TILE COLLISION DATA ----
    std::vector<int> groundTiles;  // GID của layer "ground"
    int tileWidth = 16;
    int tileHeight = 16;

    float getmapWidth() const  { return mapWidth; }
    float getmapHeight() const { return mapHeight; }

    sf::Vector2f checkCollision(const sf::Rect<float>& box, const sf::Vector2f& vel);
    std::vector<TilesetInfo> tilesets;

private:

    float mapWidth = 0;
    float mapHeight = 0;
    float tileSize = 0;

    std::vector<sf::Sprite> tiles; // background + decoration + ground visual

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};