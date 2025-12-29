#pragma once
#include <filesystem>
#include <string>

class SaveManager {
public:
    static SaveManager& get();

    // BƯỚC 1: chỉ init path + tạo folder (nếu cần)
    bool init();

    // debug để test nhanh
    const std::filesystem::path& getSaveFilePath() const { return saveFilePath; }

private:
    SaveManager() = default;

    std::filesystem::path computePrimaryPath_AppData() const;
    std::filesystem::path computeFallbackPath_ExeDir() const;

private:
    std::filesystem::path saveFilePath; // đường dẫn file save chính
};