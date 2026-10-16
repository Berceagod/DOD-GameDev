#include <vector>
#include <cmath>
#include <iostream>
#include "miscarerandom.cpp"

void ResolveCollision(std::vector<Bila>& balls, int iterations = 2, float restitution = 1.0f)
{
    const int n = (int)balls.size();
    if (n < 2) return;
 
    // reused between frames: no allocations after the first call
    static std::vector<float> x, y, vx, vy, r, invM;
    static std::vector<int> cellOf, cellStart, cursor, order;
    static std::vector<std::pair<int,int>> pairs;
 
    x.resize(n); y.resize(n); vx.resize(n); vy.resize(n); r.resize(n); invM.resize(n);
    cellOf.resize(n); order.resize(n);
 
    float maxR = 0.0f;
    for (int i = 0; i < n; i++) {
        x[i] = balls[i].getx();       y[i] = balls[i].gety();
        vx[i] = balls[i].getxspeed(); vy[i] = balls[i].getyspeed();
        r[i] = balls[i].getwidth();
        invM[i] = 1.0f / (r[i] * r[i]);
        maxR = std::max(maxR, r[i]);
    }
 
    // grid sized from the world
    float cell = 2.0f * maxR;
    int cols = std::max(1, (int)std::ceil(WORLD_W / cell));
    int rows = std::max(1, (int)std::ceil(WORLD_H / cell));
    while ((long long)cols * rows > 4000000) {   // safety cap
        cell *= 2.0f;
        cols = std::max(1, (int)std::ceil(WORLD_W / cell));
        rows = std::max(1, (int)std::ceil(WORLD_H / cell));
    }
    const int cellCount = cols * rows;
 
    // counting sort of balls into cells
    cellStart.assign(cellCount + 1, 0);
    for (int i = 0; i < n; i++) {
        int cx = std::min(std::max((int)(x[i] / cell), 0), cols - 1);
        int cy = std::min(std::max((int)(y[i] / cell), 0), rows - 1);
        cellOf[i] = cy * cols + cx;
        cellStart[cellOf[i] + 1]++;
    }
    for (int c = 0; c < cellCount; c++) cellStart[c + 1] += cellStart[c];
    cursor.assign(cellStart.begin(), cellStart.end() - 1);
    for (int i = 0; i < n; i++) order[cursor[cellOf[i]]++] = i;
 
    // find overlapping pairs
    pairs.clear();
    auto test = [&](int i, int j) {
        float dx = x[j] - x[i], dy = y[j] - y[i];
        float rs = r[i] + r[j];
        if (dx * dx + dy * dy < rs * rs) pairs.push_back({i, j});
    };
    const int fdx[4] = { 1, -1, 0, 1 };   // E, SW, S, SE
    const int fdy[4] = { 0,  1, 1, 1 };
    for (int cy = 0; cy < rows; cy++) {
        for (int cx = 0; cx < cols; cx++) {
            const int c = cy * cols + cx;
            const int s = cellStart[c], e = cellStart[c + 1];
            if (s == e) continue;
            for (int a = s; a < e; a++) {
                for (int b = a + 1; b < e; b++) test(order[a], order[b]);       // same cell
                for (int k = 0; k < 4; k++) {                                   // forward neighbours
                    int nx = cx + fdx[k], ny = cy + fdy[k];
                    if (nx < 0 || nx >= cols || ny >= rows) continue;
                    int nc = ny * cols + nx;
                    for (int b = cellStart[nc]; b < cellStart[nc + 1]; b++) test(order[a], order[b]);
                }
            }
        }
    }
 
    // resolve (a few passes make dense clusters more stable)
    for (int it = 0; it < iterations; it++) {
        for (auto& p : pairs) {
            int i = p.first, j = p.second;
            float dx = x[j] - x[i], dy = y[j] - y[i];
            float rs = r[i] + r[j];
            float d2 = dx * dx + dy * dy;
            if (d2 >= rs * rs) continue;                 // already separated by an earlier pass
 
            float d = std::sqrt(d2);
            float nx, ny;
            if (d < 1e-6f) { nx = 1.0f; ny = 0.0f; d = 0.0f; }   // exactly on top of each other
            else           { nx = dx / d; ny = dy / d; }
 
            float invSum = invM[i] + invM[j];
 
            // separate
            float corr = (rs - d) / invSum;
            x[i] -= nx * corr * invM[i];  y[i] -= ny * corr * invM[i];
            x[j] += nx * corr * invM[j];  y[j] += ny * corr * invM[j];
 
            // bounce
            float vn = (vx[j] - vx[i]) * nx + (vy[j] - vy[i]) * ny;
            if (vn > 0.0f) continue;                     // moving apart
            float imp = -(1.0f + restitution) * vn / invSum;
            vx[i] -= imp * invM[i] * nx;  vy[i] -= imp * invM[i] * ny;
            vx[j] += imp * invM[j] * nx;  vy[j] += imp * invM[j] * ny;
        }
    }
 
    for (int i = 0; i < n; i++) {
        balls[i].setPos(x[i], y[i]);
        balls[i].setSpeed(vx[i], vy[i]);
    }
}   

int test(void)
{
    srand((unsigned)time(0));
    Color green = {20, 160, 133, 255};
 
    InitWindow(WORLD_W, WORLD_H, "Example");
    SetTargetFPS(60);
 
    std::vector<Bila> bileVector;
    bileVector.reserve(BALL_COUNT);
    for (int i = 0; i < BALL_COUNT; i++) bileVector.push_back(Bila());
 
    while (WindowShouldClose() == false)
    {
        for (auto& bila : bileVector) bila.moveBalls();
        ResolveCollision(bileVector);
 
        BeginDrawing();
        ClearBackground(green);
        for (auto& bila : bileVector)
            DrawCircleV({bila.getx(), bila.gety()}, bila.getwidth(), WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }
 
    CloseWindow();
    return 0;
}