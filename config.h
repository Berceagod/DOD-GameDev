#pragma once
#include <raylib.h>

// Spawn method, chosen in the GUI BEFORE clicking to spawn balls
enum class SpawnMode
{
    OOP,
    DOD
};

namespace Config
{
    // Window
    inline constexpr int   SCREEN_WIDTH  = 1000;
    inline constexpr int   SCREEN_HEIGHT = 800;
    inline constexpr char  WINDOW_TITLE[] = "Tema";
    inline constexpr Color BACKGROUND    = {0, 0, 0, 255};
    inline constexpr int   TARGET_FPS    = 60;
    // Ball defaults
    inline constexpr float BALL_RADIUS   = 20.0f;
    inline constexpr int   BALL_SPEED    = 3;
    inline constexpr float BALL_COLOR[3] = {1.0f, 1.0f, 1.0f};  // RGB, 0..1 (ImGui format)
    inline constexpr int   BALL_START_X  = SCREEN_WIDTH / 2;
    inline constexpr int   BALL_START_Y  = SCREEN_HEIGHT / 2;

    // GUI slider ranges
    inline constexpr int   SPEED_MIN     = 1;
    inline constexpr int   SPEED_MAX     = 10;
    inline constexpr float RADIUS_MIN    = 5.0f;
    inline constexpr float RADIUS_MAX    = 100.0f;

    // Spawning
    inline constexpr SpawnMode DEFAULT_SPAWN_MODE = SpawnMode::OOP;
}