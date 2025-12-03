#pragma once
#include <SFML/Graphics.hpp>

namespace Config {
    // Player
    inline constexpr float PLAYER_SPEED = 300.f;
    inline constexpr float PLAYER_JUMP_HEIGHT = 600.f;
    inline constexpr float PLAYER_GRAVITY = 1200.f;
    inline constexpr float PLAYER_MAX_FALL_SPEED = 900.f;
    inline constexpr int   PLAYER_HEALTH = 100;
    inline constexpr float TRAP_COOLDOWN = 1.f; // giây

    // TileMap
    inline constexpr float TILE_SIZE = 50.f;

    // Camera
    inline constexpr float CAMERA_WIDTH = 800.f;
    inline constexpr float CAMERA_HEIGHT = 600.f;

    // Coin
    inline constexpr float COIN_RADIUS = 15.f; // nếu cần hitbox khác
}
