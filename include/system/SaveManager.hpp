#pragma once
#include <filesystem>
#include <string>

class SaveManager {
public:
    static SaveManager& get();

    // khởi tạo hệ thống save, trả về true nếu dùng được AppData, false nếu fallback
    bool init();

    // debug để test nhanh
    const std::filesystem::path& getSaveFilePath() const { return saveFilePath; }

    // load/save dữ liệu
    void load();
    void save();
    // lấy/đặt level mở khóa tối đa
    int  getMaxUnlockedLevel() const { return maxUnlockedLevel; }

private:
    SaveManager() = default;

    std::filesystem::path computePrimaryPath_AppData() const;
    std::filesystem::path computeFallbackPath_ExeDir() const;

private:
    std::filesystem::path saveFilePath; // đường dẫn file save chính
    int maxUnlockedLevel = 1; // level mặc định
};