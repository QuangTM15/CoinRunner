#pragma once
#include <SFML/Graphics.hpp>

namespace Config {
    // Player
    inline constexpr float PLAYER_SPEED = 48.f;
    inline constexpr float PLAYER_JUMP_HEIGHT = 80.f;
    inline constexpr float PLAYER_GRAVITY = 1400.f;
    inline constexpr float PLAYER_MAX_FALL_SPEED = 900.f;
    inline constexpr float PLAYER_SCALE = 2.f;
    inline constexpr float PLAYER_CONTROL_LOCK_TIME = 0.1f;
    inline constexpr int PLAYER_INITIAL_LIFE = 7;
    inline constexpr float PLAYER_INVINCIBILITY_TIME = 1.0f;



    // TileMap
    inline constexpr float TILE_SIZE = 16.f;

    // Camera
    inline constexpr float CAMERA_WIDTH = 720.f;
    inline constexpr float CAMERA_HEIGHT = 1280.f;

    // Coin
    inline constexpr float COIN_RADIUS = 15.f; // nếu cần hitbox khác
    // =========================
    // GAME LOGIC RESOLUTION
    // =========================
    static constexpr float GAME_W = 1280.f; // 20 tiles * 16px
    static constexpr float GAME_H = 720.f; // chuẩn indie platformer
}
