#include "system/SaveManager.hpp"
#include <iostream>
#include <json/json.hpp>
#include <filesystem>
#include <fstream>

using json = nlohmann::json;

#ifdef _WIN32
    #include <cstdlib> // for std::getenv
    #include <windows.h>
#endif

SaveManager& SaveManager::get() {
    static SaveManager instance;
    return instance;
}

bool SaveManager::init() {
    // 1) thử AppData/Local/CoinRunner/save.json
    auto primary = computePrimaryPath_AppData();

    // 2) nếu primary hợp lệ -> dùng primary, else fallback cạnh exe
    std::error_code ec;

    if (!primary.empty()) {
        auto dir = primary.parent_path();
        std::filesystem::create_directories(dir, ec);

        if (!ec) {
            saveFilePath = primary;
            std::cout << "[Save] Using AppData: " << saveFilePath.string() << "\n";
            return true;
        }
        std::cout << "[Save] AppData create dir failed, fallback. Error: " << ec.message() << "\n";
    }

    auto fallback = computeFallbackPath_ExeDir();
    auto dir2 = fallback.parent_path();
    ec.clear();
    std::filesystem::create_directories(dir2, ec); // thường dir2 đã tồn tại
    if (ec) {
        std::cout << "[Save] Fallback create dir failed. Error: " << ec.message() << "\n";
        // vẫn set path để còn thử ghi sau (bước sau sẽ xử lý kỹ hơn)
    }

    saveFilePath = fallback;
    std::cout << "[Save] Using Fallback (exe dir): " << saveFilePath.string() << "\n";
    return false;
}

std::filesystem::path SaveManager::computePrimaryPath_AppData() const
{
#ifdef _WIN32
    const char* env = std::getenv("LOCALAPPDATA");
    if (env && env[0] != '\0')
    {
        std::filesystem::path base(env);
        return base / "CoinRunner" / "save.json";
    }
#endif
    return {};
}

std::filesystem::path SaveManager::computeFallbackPath_ExeDir() const {
#ifdef _WIN32
    char buf[MAX_PATH] = {0};
    DWORD n = GetModuleFileNameA(NULL, buf, MAX_PATH);
    if (n > 0 && n < MAX_PATH) {
        std::filesystem::path exePath(buf);
        auto exeDir = exePath.parent_path();
        return exeDir / "save.json";
    }
#endif
    // fallback cuối: current working directory
    return std::filesystem::current_path() / "save.json";
}

void SaveManager::load()
{
    // nếu chưa tồn tại → tạo file mặc định
    if (!std::filesystem::exists(saveFilePath))
    {
        std::cout << "[Save] No save file. Create default.\n";
        maxUnlockedLevel = 1;
        save();
        return;
    }

    std::ifstream in(saveFilePath);
    if (!in.is_open())
    {
        std::cout << "[Save] Cannot open save file. Use default.\n";
        maxUnlockedLevel = 1;
        return;
    }

    try
    {
        json j;
        in >> j;
        maxUnlockedLevel = j.value("maxUnlockedLevel", 1);
        std::cout << "[Save] Loaded. maxUnlockedLevel = "
                  << maxUnlockedLevel << "\n";
    }
    catch (...)
    {
        std::cout << "[Save] Save file corrupted. Reset.\n";
        maxUnlockedLevel = 1;
        save();
    }
}

void SaveManager::save()
{
    json j;
    j["maxUnlockedLevel"] = maxUnlockedLevel;

    std::ofstream out(saveFilePath);
    if (!out.is_open())
    {
        std::cout << "[Save] Cannot write save file!\n";
        return;
    }

    out << j.dump(4);
    std::cout << "[Save] Saved. maxUnlockedLevel = "
              << maxUnlockedLevel << "\n";
}

void SaveManager::unlockLevel(int level)
{
    if (level <= maxUnlockedLevel)
        return;

    maxUnlockedLevel = level;
    save();
}