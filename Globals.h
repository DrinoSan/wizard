#pragma once

#include <cstdint>

constexpr int32_t screenWidth  = 800;
constexpr int32_t screenHeight = 450;

// Map
constexpr int   MAP_TILES_X     = 30;
constexpr int   MAP_TILES_Y     = 20;
constexpr float FIXED_TILE_SIZE = 32.0f;
constexpr float WORLD_WIDTH     = MAP_TILES_X * FIXED_TILE_SIZE;   // e.g. 960
constexpr float WORLD_HEIGHT    = MAP_TILES_Y * FIXED_TILE_SIZE;   // e.g. 640

constexpr float   PLAYER_MOVEMENT_SPEED{ 1.5f };
constexpr int32_t MAX_ACTIVE_ATTACKS{ 32 };

constexpr int32_t NUM_ENEMIES{ 1 };
