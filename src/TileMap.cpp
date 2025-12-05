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

    std::cout << "\n[TSX] Loading TSX: " << tsxPath << "\n";

    std::ifstream f(tsxPath);
    if (!f.is_open()) {
        std::cout << "[TSX] ❌ Cannot open TSX: " << tsxPath << "\n";
        return info;
    }

    std::string xml((std::istreambuf_iterator<char>(f)), {});
    std::cout << "[TSX] XML size = " << xml.size() << " bytes\n";

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

// -------- LOAD JSON MAP --------
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

            // --- SPRITE SFML 3 ---
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

    return true;
}

// ----------- DRAW ----------- 
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& s : tiles)
        target.draw(s, states);
}
