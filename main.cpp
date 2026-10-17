#include <raylib.h>
#include "imgui.h"
#include "rlImGui.h"
#include "config.h"
#include "benchmark.h"

int main(void)
{

    int   ballX      = Config::BALL_START_X;
    int   ballY      = Config::BALL_START_Y;
    int   speed      = Config::BALL_SPEED;
    float ballRadius = Config::BALL_RADIUS;
    float ballColor[3] = {Config::BALL_COLOR[0], Config::BALL_COLOR[1], Config::BALL_COLOR[2]};
    SpawnMode mode   = Config::DEFAULT_SPAWN_MODE;
    bool showDemo    = false;



    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Config::WINDOW_TITLE);
    SetTargetFPS(Config::TARGET_FPS);
    rlImGuiSetup(true);

    while (!WindowShouldClose())
    {
        Benchmark::Begin();

        if (IsKeyDown(KEY_RIGHT)) ballX += speed;
        if (IsKeyDown(KEY_LEFT))  ballX -= speed;
        if (IsKeyDown(KEY_UP))    ballY -= speed;
        if (IsKeyDown(KEY_DOWN))  ballY += speed;

        BeginDrawing();
        ClearBackground(Config::BACKGROUND);

        Color c = {
            (unsigned char)(ballColor[0] * 255),
            (unsigned char)(ballColor[1] * 255),
            (unsigned char)(ballColor[2] * 255),
            255
        };
        DrawCircle(ballX, ballY, ballRadius, c);
        Benchmark::End();

        // ---- GUI ----
        rlImGuiBegin();
        Benchmark::DrawUI();
        
        ImGui::Begin("Ball Controls");
        ImGui::SliderInt("Speed", &speed, 1, 10);
        ImGui::SliderFloat("Radius", &ballRadius, 5.0f, 100.0f);
        ImGui::ColorEdit3("Color", ballColor);
        if (ImGui::Button("Reset position"))
        {
            ballX = Config::BALL_START_X;
            ballY = Config::BALL_START_Y;
        }
        ImGui::Checkbox("Show demo window", &showDemo);
        ImGui::Text("Position: %d, %d", ballX, ballY);
        ImGui::End();

        if (showDemo) ImGui::ShowDemoWindow(&showDemo);

        rlImGuiEnd(); 

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}