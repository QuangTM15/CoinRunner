#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include "TileSetInfo.hpp"

class TileMap : public sf::Drawable {
public:
    // --- MAP OBJECT ---
    struct MapObject {
        sf::Rect<float> rect;
        int gid = 0; // 0 = region (spawn, killzone, checkpoint...), >0 = tile object (coin, trap, spider)
        std::unordered_map<std::string, float> floatProps;
    };

    TileMap() = default;
    bool loadFromFile(const std::string& mapFile, float tileSize);

    sf::Vector2f spawnPoint;

    std::vector<sf::Rect<float>> ladderAreas;
    std::vector<sf::Rect<float>> killzones;
    std::vector<sf::Rect<float>> checkpoints;
    std::vector<MapObject> trapDamages;  // type = trap_damage
    std::vector<MapObject> coins;        // type = coin
    std::vector<MapObject> spiders;      // type = spider
    float getmapWidth() const { return mapWidth; }
    float getmapHeight() const { return mapHeight; }

    // --- TILEMAP COLLISION SYSTEM ---
    std::vector<int> groundTiles;     // mảng GID của layer ground
    int tileWidth = 16;
    int tileHeight = 16;

    sf::Vector2i getTileCoords(int index) const;
    bool isSolid(int gid) const;

    sf::Vector2f checkCollision(const sf::Rect<float>& box, const sf::Vector2f& vel);


private:
    float mapWidth = 0, mapHeight = 0;
    float tileSize = 0;

    std::vector<TilesetInfo> tilesets;
    std::vector<sf::Sprite> tiles;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};