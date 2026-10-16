#include <raylib.h>
#include "imgui.h"
#include "rlImGui.h"

int main(void)
{
    int ballX = 400;
    int ballY = 400;
    int speed = 3;
    float ballRadius = 20.0f;
    float ballColor[3] = {1.0f, 1.0f, 1.0f};
    bool showDemo = false;

    Color green = {20, 160, 133, 255};

    InitWindow(800, 800, "Example");
    SetTargetFPS(60);
    rlImGuiSetup(true);

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_RIGHT)) ballX += speed;
        if (IsKeyDown(KEY_LEFT))  ballX -= speed;
        if (IsKeyDown(KEY_UP))    ballY -= speed;
        if (IsKeyDown(KEY_DOWN))  ballY += speed;

        BeginDrawing();
        ClearBackground(green);

        Color c = {
            (unsigned char)(ballColor[0] * 255),
            (unsigned char)(ballColor[1] * 255),
            (unsigned char)(ballColor[2] * 255),
            255
        };
        DrawCircle(ballX, ballY, ballRadius, c);

        // ---- GUI ----
        rlImGuiBegin();

        ImGui::Begin("Ball Controls");
        ImGui::SliderInt("Speed", &speed, 1, 10);
        ImGui::SliderFloat("Radius", &ballRadius, 5.0f, 100.0f);
        ImGui::ColorEdit3("Color", ballColor);
        if (ImGui::Button("Reset position"))
        {
            ballX = 400;
            ballY = 400;
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