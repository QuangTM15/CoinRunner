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

    mapWidth  = j["width"];
    mapHeight = j["height"];
    tileWidth  = j["tilewidth"];
    tileHeight = j["tileheight"];

    std::filesystem::path mapDir = std::filesystem::path(mapFile).parent_path();

    // ============================
    // LOAD TILESETS
    // ============================
    tilesets.clear();
    for (auto& ts : j["tilesets"])
    {
        int firstgid = ts["firstgid"];
        std::string tsxPath = (mapDir / ts["source"].get<std::string>()).string();
        tilesets.push_back(loadTsx(tsxPath, firstgid));
    }

    std::sort(tilesets.begin(), tilesets.end(),
        [](const TilesetInfo& a, const TilesetInfo& b) {
            return a.firstgid < b.firstgid;
        }
    );

    auto getTileset = [&](int gid) -> const TilesetInfo*
    {
        gid &= 0x1FFFFFFF;
        const TilesetInfo* t = nullptr;
        for (auto& ts : tilesets)
        {
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

            int gid = rawGid & 0x1FFFFFFF;
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

            sprite.setPosition(sf::Vector2f(x * tileWidth, y * tileHeight));
            tiles.push_back(sprite);
        }
    }

    // ===============================
    // GROUND COLLISION LAYER
    // ===============================
    for (auto& layer : j["layers"])
    {
        if (layer.value("name","") == "ground" && layer["type"] == "tilelayer")
        {
            groundTiles = layer["data"].get<std::vector<int>>();
        }
    }

    // ===============================
    // LOAD OBJECTS (NEW CLEAN VERSION)
    // ===============================

    spawnPoint = {0,0};
    coins.clear();
    trapsStatic.clear();
    trapsMoving.clear();
    checkpoints.clear();
    killzones.clear();

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

            sf::Vector2f topLeft(x, y - h);
            sf::Rect<float> rect(topLeft, { w, h });

            int gid = obj.value("gid", 0);

            // ---- SPAWN ----
            if ( type == "spawn")
            {
                spawnPoint = topLeft;
            }
            // ---- COIN ----
            else if (type == "coin")
            {
                MapObject c{ rect, gid };
                coins.push_back(c);
            }
            // ---- STATIC TRAP ----
            else if (type == "st")
            {
                MapObject t{ rect, gid };
                trapsStatic.push_back(t);
            }
            // ---- MOVING TRAP ----
            else if (type == "mv")
            {
                MapObject t{ rect, gid };

                for (auto& p : obj.value("properties", json::array()))
                {
                    std::string key = p.value("name", "");

                    if (key == "range")
                        t.floatProps["range"] = p.value("value", 0.f);

                    else if (key == "axis")
                        t.floatProps["axis"] = (p.value("value", "x") == "x" ? 0.f : 1.f);
                }
                trapsMoving.push_back(t);
            }
            // ---- CHECKPOINT ----
            else if (type == "checkpoint")
            {
                MapObject cp{ rect, gid };
                checkpoints.push_back(cp);
            }
            // ---- KILLZONE ----
            else if (type == "killzone")
            {
                killzones.push_back(rect);
            }
        }
    }

    return true; // 🔥 FIRE — PHẢI CÓ DÒNG NÀY
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
    sf::Vector2f fix(0.f, 0.f);

    // ============================================================
    // A. HANDLE X AXIS
    // ============================================================
    if (vel.x != 0.f)
    {
        float newLeft   = box.position.x + vel.x;
        float newRight  = newLeft + box.size.x;

        float top    = box.position.y;
        float bottom = box.position.y + box.size.y - 0.01f;

        // tile range
        int leftTile   = (int)(newLeft / tileWidth);
        int rightTile  = (int)((newRight - 0.01f) / tileWidth);
        int topTile    = (int)(top / tileHeight);
        int bottomTile = (int)(bottom / tileHeight);

        bool collided  = false;
        float bestFix = 0.f;

        for (int ty = topTile; ty <= bottomTile; ++ty)
        for (int tx = leftTile; tx <= rightTile; ++tx)
        {
            if (tx < 0 || ty < 0 || tx >= mapWidth || ty >= mapHeight)
                continue;

            int gid = groundTiles[ty * mapWidth + tx] & 0x1FFFFFFF;
            if (gid == 0) continue;

            float tileLeft   = tx * tileWidth;
            float tileRight  = tileLeft + tileWidth;

            float fixCandidate;

            if (vel.x > 0) // moving right
            {
                fixCandidate = tileLeft - newRight;  // negative value

                if (!collided || fixCandidate > bestFix)
                    bestFix = fixCandidate;
            }
            else // moving left
            {
                fixCandidate = tileRight - newLeft;  // positive value

                if (!collided || fixCandidate < bestFix)
                    bestFix = fixCandidate;
            }

            collided = true;
        }

        if (collided)
            fix.x = bestFix;
    }

    // ============================================================
    // B. HANDLE Y AXIS
    // ============================================================
    if (vel.y != 0.f)
    {
        float newTop    = box.position.y + vel.y;
        float newBottom = newTop + box.size.y;

        float left  = box.position.x + fix.x;                 // apply X correction
        float right = left + box.size.x - 0.01f;

        int leftTile   = (int)(left / tileWidth);
        int rightTile  = (int)(right / tileWidth);
        int topTile    = (int)(newTop / tileHeight);
        int bottomTile = (int)((newBottom - 0.01f) / tileHeight);

        bool collided = false;
        float bestFix = 0.f;

        for (int ty = topTile; ty <= bottomTile; ++ty)
        for (int tx = leftTile; tx <= rightTile; ++tx)
        {
            if (tx < 0 || ty < 0 || tx >= mapWidth || ty >= mapHeight)
                continue;

            int gid = groundTiles[ty * mapWidth + tx] & 0x1FFFFFFF;
            if (gid == 0) continue;

            float tileTop    = ty * tileHeight;
            float tileBottom = tileTop + tileHeight;

            float fixCandidate;

            if (vel.y > 0) // moving down
            {
                fixCandidate = tileTop - newBottom;   // negative

                if (!collided || fixCandidate > bestFix)
                    bestFix = fixCandidate;
            }
            else // moving up
            {
                fixCandidate = tileBottom - newTop;   // positive

                if (!collided || fixCandidate < bestFix)
                    bestFix = fixCandidate;
            }

            collided = true;
        }

        if (collided)
            fix.y = bestFix;
    }

    return fix;
}