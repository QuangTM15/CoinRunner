#include "TileMap.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>

using json = nlohmann::json;

// =======================================================
// LOAD TSX (không đổi)
// =======================================================
TilesetInfo loadTsx(const std::string& tsxPath, int firstgid)
{
    TilesetInfo info;
    info.firstgid = firstgid;

    std::ifstream f(tsxPath);
    if (!f.is_open()) {
        std::cout << "[TSX] ❌ Cannot open TSX file: " << tsxPath << "\n";
        return info;
    }

    std::string xml((std::istreambuf_iterator<char>(f)), {});
    size_t posCol = xml.find("columns=\"");
    if (posCol != std::string::npos) {
        posCol += 9;
        size_t posColEnd = xml.find("\"", posCol);
        info.columns = std::stoi(xml.substr(posCol, posColEnd - posCol));
    }

    size_t posImg = xml.find("image source=\"");
    if (posImg != std::string::npos) {
        posImg += strlen("image source=\"");
        size_t posImgEnd = xml.find("\"", posImg);
        info.imagePath = xml.substr(posImg, posImgEnd - posImg);
    }

    std::filesystem::path dir = std::filesystem::path(tsxPath).parent_path();
    info.imagePath = (dir / info.imagePath).string();
    info.texture.loadFromFile(info.imagePath);

    return info;
}

// =======================================================
// LOAD MAP
// =======================================================
bool TileMap::loadFromFile(const std::string& mapFile, float tileSize)
{
    std::cout << "\n[MAP] Loading map: " << mapFile << "\n";
    this->tileSize = tileSize;

    std::ifstream file(mapFile);
    if (!file.is_open()) return false;

    json j;
    file >> j;

    mapWidth = j["width"];
    mapHeight = j["height"];
    tileWidth = j["tilewidth"];
    tileHeight = j["tileheight"];

    std::filesystem::path mapDir = std::filesystem::path(mapFile).parent_path();

    // load tilesets
    tilesets.clear();
    for (auto& ts : j["tilesets"]) {
        int firstgid = ts["firstgid"];
        std::string tsxPath = (mapDir / ts["source"].get<std::string>()).string();
        tilesets.push_back(loadTsx(tsxPath, firstgid));
    }

    std::sort(tilesets.begin(), tilesets.end(),
        [](const TilesetInfo& a, const TilesetInfo& b) { return a.firstgid < b.firstgid; });

    auto getTileset = [&](int gid) -> const TilesetInfo*
    {
        gid = gid & 0x1FFFFFFF; // REMOVE FLIP BITS
        const TilesetInfo* t = nullptr;
        for (auto& ts : tilesets) {
            if (gid >= ts.firstgid)
                t = &ts;
            else break;
        }
        return t;
    };

    // ============================
    // LOAD TILE LAYERS
    // ============================
    tiles.clear();

    for (auto& layer : j["layers"])
    {
        if (layer["type"] != "tilelayer") continue;

        const auto& data = layer["data"];
        int index = 0;

        for (int y = 0; y < mapHeight; y++)
        for (int x = 0; x < mapWidth; x++)
        {
            int rawGid = data[index++];
            if (rawGid == 0) continue;

            int gid = rawGid & 0x1FFFFFFF; // remove flip bits
            const TilesetInfo* ts = getTileset(gid);
            if (!ts) continue;

            int local = gid - ts->firstgid;
            int tx = local % ts->columns;
            int ty = local / ts->columns;

            sf::Sprite sprite(ts->texture);
            sprite.setTextureRect({
                sf::Vector2i(tx * tileWidth, ty * tileHeight),
                sf::Vector2i(tileWidth, tileHeight)
            });

            sprite.setPosition({ x * tileWidth, y * tileHeight });
            tiles.push_back(sprite);
        }
    }

    // ============================
    // LOAD GROUND COLLISION LAYER
    // ============================
    for (auto& layer : j["layers"])
    {
        if (layer.value("name", "") == "ground" && layer["type"] == "tilelayer")
        {
            groundTiles = layer["data"].get<std::vector<int>>();
        }
    }

    // ============================
    // LOAD OBJECTS (FIX Y OFFSET)
    // ============================
    spawnPoint = { 0,0 };
    ladderAreas.clear();
    killzones.clear();
    checkpoints.clear();
    trapDamages.clear();
    coins.clear();
    spiders.clear();

    for (auto& layer : j["layers"])
    {
        if (layer["type"] != "objectgroup") continue;

        for (auto& obj : layer["objects"])
        {
            std::string type = obj.value("type", "");

            float x = obj.value("x", 0.f);
            float y = obj.value("y", 0.f);
            float w = obj.value("width", 0.f);
            float h = obj.value("height", 0.f);

            // FIX: convert bottom-left origin to tile top-left origin
            sf::Vector2f topLeft(x, y - h - tileHeight);

            sf::Rect<float> rect(topLeft, {w, h});

            if (type == "spawn")
            {
                spawnPoint = topLeft;
            }
            else if (type == "LadderArea")
                ladderAreas.push_back(rect);
            else if (type == "killzone")
                killzones.push_back(rect);
            else if (type == "checkpoint")
                checkpoints.push_back(rect);
            else if (type == "coin")
            {
                MapObject c; c.rect = rect; coins.push_back(c);
            }
            else if (type == "trap_damage")
            {
                MapObject t; t.rect = rect; trapDamages.push_back(t);
            }
            else if (type == "spider")
            {
                MapObject s; s.rect = rect; spiders.push_back(s);
            }
        }
    }

    return true;
}

// =======================================================
// DRAW
// =======================================================
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& s : tiles)
        target.draw(s, states);
}

// =======================================================
// COLLISION SYSTEM (FIX TILE INDEXING)
// =======================================================
sf::Vector2f TileMap::checkCollision(const sf::Rect<float>& box, const sf::Vector2f& vel)
{
    sf::Vector2f fix(0,0);

    // ============================
    // Build NEW box after movement
    // ============================
    sf::Rect<float> newBox = box;
    newBox.position.x += vel.x;
    newBox.position.y += vel.y;

    float leftNew   = newBox.position.x;
    float rightNew  = newBox.position.x + newBox.size.x;
    float topNew    = newBox.position.y;
    float bottomNew = newBox.position.y + newBox.size.y;

    // ========== X AXIS ==========
    if (vel.x != 0.f)
    {
        float newLeft   = box.position.x + vel.x;
        float newRight  = newLeft + box.size.x;

        float top    = box.position.y;
        float bottom = box.position.y + box.size.y - 0.01f;

        int leftTile   = (int)(newLeft / tileWidth);
        int rightTile  = (int)((newRight - 0.01f) / tileWidth);
        int topTile    = (int)(top / tileHeight);
        int bottomTile = (int)(bottom / tileHeight);

        float bestFix = 0.f;
        bool collided = false;

        for (int ty = topTile; ty <= bottomTile; ++ty)
        for (int tx = leftTile; tx <= rightTile; ++tx)
        {
            if (tx < 0 || ty < 0 || tx >= mapWidth || ty >= mapHeight) continue;

            int gid = groundTiles[ty * mapWidth + tx] & 0x1FFFFFFF;
            if (gid == 0) continue;

            collided = true;

            if (vel.x > 0)
                bestFix = (tx * tileWidth) - newRight;
            else
                bestFix = ((tx + 1) * tileWidth) - newLeft;
        }

        if (collided)
            fix.x = bestFix;
    }


    // ========== Y AXIS ==========
    if (vel.y != 0.f)
    {
        float topY    = topNew;
        float bottomY = bottomNew - 0.01f;

        int left   = (int)(leftNew / tileWidth);
        int right  = (int)((rightNew - 0.01f) / tileWidth);
        int top    = (int)(topY / tileHeight);
        int bottom = (int)(bottomY / tileHeight);

        float bestFix = 0.f;
        bool hit = false;

        for (int ty = top; ty <= bottom; ty++)
        for (int tx = left; tx <= right; tx++)
        {
            if (tx < 0 || ty < 0 || tx >= mapWidth || ty >= mapHeight) continue;

            int gid = groundTiles[ty * mapWidth + tx] & 0x1FFFFFFF;
            if (gid == 0) continue;

            hit = true;

            if (vel.y > 0)
                bestFix = (ty * tileHeight) - bottomNew;
            else
                bestFix = ((ty + 1) * tileHeight) - topNew;
        }

        if (hit) fix.y = bestFix;
    }

    return fix;
}