#pragma once
#include <SFML/Graphics.hpp>
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

private:
    float mapWidth = 0, mapHeight = 0;
    float tileSize = 0;

    std::vector<TilesetInfo> tilesets;
    std::vector<sf::Sprite> tiles;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};