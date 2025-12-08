#include "TileMap.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>

using json = nlohmann::json;

// -------- LOAD TSX --------
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


    // columns
    size_t posCol = xml.find("columns=\"");
    if (posCol == std::string::npos)
        std::cout << "[TSX] ❌ Cannot find columns attribute!\n";
    else {
        posCol += 9;
        size_t posColEnd = xml.find("\"", posCol);
        info.columns = std::stoi(xml.substr(posCol, posColEnd - posCol));

        std::cout << "[TSX] columns = " << info.columns << "\n";
    }

    // image
    size_t posImg = xml.find("image source=\"");
    if (posImg == std::string::npos)
        std::cout << "[TSX] ❌ Cannot find <image source> !\n";
    else {
        posImg += strlen("image source=\"");
        size_t posImgEnd = xml.find("\"", posImg);
        info.imagePath = xml.substr(posImg, posImgEnd - posImg);
        std::cout << "[TSX] image source = " << info.imagePath << "\n";
    }

    std::filesystem::path dir = std::filesystem::path(tsxPath).parent_path();
    info.imagePath = (dir / info.imagePath).string();
    std::cout << "[TSX] Full PNG path = " << info.imagePath << "\n";

    if (!info.texture.loadFromFile(info.imagePath)) {
        std::cout << "[TSX] ❌ Failed to load PNG\n";
    } else {
        std::cout << "[TSX] ✔ PNG loaded. Size = "
            << info.texture.getSize().x << "x"
            << info.texture.getSize().y << "\n";
    }

    return info;
}

// -------- LOAD MAP --------
bool TileMap::loadFromFile(const std::string& mapFile, float tileSize)
{
    std::cout << "\n[MAP] Loading map: " << mapFile << "\n";
    this->tileSize = tileSize;

    std::ifstream file(mapFile);
    if (!file.is_open()) {
        std::cout << "Opening file: " << std::filesystem::absolute(mapFile) << "\n";
        std::cout << "Cannot open map file!\n";
        return false;
    }

    json j;
    file >> j;

    mapWidth = j["width"];
    mapHeight = j["height"];
    std::cout << "[MAP] Size = " << mapWidth << " x " << mapHeight << "\n";

    tilesets.clear();
    std::filesystem::path mapDir = std::filesystem::path(mapFile).parent_path();
    // =========================
    //      LOAD TILESET
    // =========================

    for (auto& ts : j["tilesets"]) {
        int firstgid = ts["firstgid"];
        std::string tsxPath = (mapDir / ts["source"].get<std::string>()).string();
        tilesets.push_back(loadTsx(tsxPath, firstgid));
    }

    std::sort(tilesets.begin(), tilesets.end(),
        [](const TilesetInfo& a, const TilesetInfo& b) {
            return a.firstgid < b.firstgid;
        });

    auto getTileset = [&](int gid) -> const TilesetInfo*
    {
        const TilesetInfo* t = nullptr;
        for (auto& ts : tilesets) {
            if (gid >= ts.firstgid)
                t = &ts;
            else break;
        }
        return t;
    };

    // =========================
    //      LOAD TILE LAYERS
    // =========================
    tiles.clear();

    for (auto& layer : j["layers"])
    {
        if (layer["type"] != "tilelayer") continue;

        const auto& data = layer["data"];
        int index = 0;

        for (int y = 0; y < mapHeight; y++)
        for (int x = 0; x < mapWidth; x++)
        {
            int gid = data[index++];
            if (gid == 0) continue;

            const TilesetInfo* ts = getTileset(gid);
            if (!ts) continue;

            int local = gid - ts->firstgid;
            int tx = local % ts->columns;
            int ty = local / ts->columns;

            sf::Sprite sprite(ts->texture);

            sprite.setTextureRect(sf::IntRect(
                sf::Vector2i(tx * (int)tileSize, ty * (int)tileSize),
                sf::Vector2i((int)tileSize, (int)tileSize)
            ));

            sprite.setPosition(sf::Vector2f(
                x * tileSize,
                y * tileSize
            ));

            tiles.push_back(sprite);
        }
    }
    // ================================
    //      LOAD GROUND COLLISION LAYER
    // ================================
    for (auto& layer : j["layers"])
    {
        std::string lname = layer.value("name", "");

        if (lname == "ground" && layer["type"] == "tilelayer")
        {
            groundTiles = layer["data"].get<std::vector<int>>();
            tileWidth = j["tilewidth"];
            tileHeight = j["tileheight"];

            std::cout << "[GROUND] size = " << groundTiles.size()
                    << " expected = " << (mapWidth * mapHeight) << "\n";
        }
    }

    // =========================
    //      LOAD OBJECTS
    // =========================
    spawnPoint = {0.f, 0.f};
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

        // Tiled object origin = bottom-left
        sf::Vector2f topLeft(x, y - h);
        sf::Vector2f center(x + w * 0.5f, y - h * 0.5f);

        sf::Vector2f size(w, h);
        sf::Rect<float> rect(topLeft, size);

        if (type == "spawn")
            spawnPoint = { x, y };
        else if (type == "LadderArea")
            ladderAreas.push_back(rect);
        else if (type == "killzone")
            killzones.push_back(rect);
        else if (type == "checkpoint")
            checkpoints.push_back(rect);
        else if (type == "coin") {
            MapObject c;
            c.rect = rect;
            c.gid = obj.value("gid", 0);
            coins.push_back(c);
        }
        else if (type == "trap_damage") {
            MapObject t;
            t.rect = rect;
            t.gid = obj.value("gid", 0);

            if (obj.contains("properties"))
                for (auto& p : obj["properties"])
                    t.floatProps[p["name"]] = p["value"];

            trapDamages.push_back(t);
        }
        else if (type == "spider") {
            MapObject s;
            s.rect = rect;
            s.gid = obj.value("gid", 0);

            if (obj.contains("properties"))
                for (auto& p : obj["properties"])
                    s.floatProps[p["name"]] = p["value"];

            spiders.push_back(s);
        }

        }
    }

    return true;
}

// ----------- DRAW ----------- 
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& s : tiles)
        target.draw(s, states);
}

// -------- COLLISION SYSTEM --------

sf::Vector2i TileMap::getTileCoords(int index) const
{
    int w = static_cast<int>(mapWidth); 
    int x = index % w;
    int y = index / w;
    return {x, y};
}

bool TileMap::isSolid(int gid) const
{
    return gid != 0;
}

sf::Vector2f TileMap::checkCollision(const sf::Rect<float>& box, const sf::Vector2f& vel)
{
    sf::Vector2f fix(0.f, 0.f);

    // ======================================================
    // 1) KIỂM TRA VA CHẠM THEO TRỤC X
    // ======================================================
    if (vel.x != 0.f)
    {
        // dự đoán vị trí mới theo X
        float newX = box.position.x + vel.x;

        int left   = (int)std::floor(newX / tileWidth);
        int right  = (int)std::floor((newX + box.size.x) / tileWidth);
        int top    = (int)std::floor(box.position.y / tileHeight);
        int bottom = (int)std::floor((box.position.y + box.size.y) / tileHeight);

        float bestFix = 0.f;
        bool collided = false;

        for (int ty = top; ty <= bottom; ++ty)
        for (int tx = left; tx <= right; ++tx)
        {
            if (tx < 0 || ty < 0 || tx >= mapWidth || ty >= mapHeight) 
                continue;

            int gid = groundTiles[ty * mapWidth + tx];
            if (!isSolid(gid)) 
                continue;

            collided = true;

            if (vel.x > 0.f)
            {
                float nf = (tx * tileWidth) - (box.position.x + box.size.x);
                if (std::abs(nf) < std::abs(bestFix) || bestFix == 0.f)
                    bestFix = nf;
            }
            else
            {
                float nf = ((tx + 1) * tileWidth) - box.position.x;
                if (std::abs(nf) < std::abs(bestFix) || bestFix == 0.f)
                    bestFix = nf;
            }
        }

        if (collided)
            fix.x = bestFix;
    }

    // ======================================================
    // 2) KIỂM TRA VA CHẠM THEO TRỤC Y
    // ======================================================
    if (vel.y != 0.f)
    {
        float newY = box.position.y + vel.y;

        int left   = (int)std::floor(box.position.x / tileWidth);
        int right  = (int)std::floor((box.position.x + box.size.x) / tileWidth);
        int top    = (int)std::floor(newY / tileHeight);
        int bottom = (int)std::floor((newY + box.size.y) / tileHeight);

        float bestFix = 0.f;
        bool collided = false;

        for (int ty = top; ty <= bottom; ++ty)
        for (int tx = left; tx <= right; ++tx)
        {
            if (tx < 0 || ty < 0 || tx >= mapWidth || ty >= mapHeight)
                continue;

            int gid = groundTiles[ty * mapWidth + tx];
            if (!isSolid(gid)) 
                continue;

            collided = true;

            if (vel.y > 0.f) // rơi xuống
            {
                float nf = (ty * tileHeight) - (box.position.y + box.size.y);
                if (std::abs(nf) < std::abs(bestFix) || bestFix == 0.f)
                    bestFix = nf;
            }
            else // nhảy lên
            {
                float nf = ((ty + 1) * tileHeight) - box.position.y;
                if (std::abs(nf) < std::abs(bestFix) || bestFix == 0.f)
                    bestFix = nf;
            }
        }

        if (collided)
            fix.y = bestFix;
    }
    return fix;
}