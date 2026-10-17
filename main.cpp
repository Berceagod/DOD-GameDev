#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "imgui.h"
#include "rlImGui.h"
#include "config.h"
#include "benchmark.h"
#include "dod_system.h"

void check_mouse(bool useDOD, DODSystem& dodSystem);
void clear_grid(std::vector<std::vector<int>>& grid);
void update_and_populate_grid(bool useDOD, DODSystem& dodSystem, std::vector<std::vector<int>>& grid, int CellSize, int cols, int rows);
void resolve_cell_collisions(bool useDOD, const std::vector<int>& cellIndexes, DODSystem& dodSystem);
void resolve_all_collisions(bool useDOD, const std::vector<std::vector<int>>& grid, DODSystem& dodSystem);
void draw_balls(bool useDOD, DODSystem& dodSystem);

int main(void)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Config::WINDOW_TITLE);
    SetTargetFPS(Config::TARGET_FPS);
    rlImGuiSetup(true);
    
    // Variabila care controleaza daca se foloseste DOD sau OOP
    bool useDOD = true; 

    // Initializare Grid
    int CellSize = 100;
    std::vector<std::vector<int>> grid; 
    int cols = Config::SCREEN_WIDTH / CellSize;
    int rows = Config::SCREEN_HEIGHT / CellSize;
    if (Config::SCREEN_WIDTH % CellSize != 0) cols++;
    if (Config::SCREEN_HEIGHT % CellSize != 0) rows++;
    grid.resize(cols * rows);

    // Initializare Sisteme
    DODSystem dodSystem;
    // OOPSystem oopSystem;

    while (!WindowShouldClose())
    {
        Benchmark::Begin();

        // spawn bile
        check_mouse(useDOD, dodSystem);

        //golire grid
        clear_grid(grid);

        //miscare si updatare grid
        update_and_populate_grid(useDOD, dodSystem, grid, CellSize, cols, rows);

        //coliziune
        resolve_all_collisions(useDOD, grid, dodSystem);

        //draw
        BeginDrawing();
        ClearBackground(Config::BACKGROUND);
        draw_balls(useDOD, dodSystem);
        Benchmark::End();

        // ---- GUI ----
        rlImGuiBegin();
        Benchmark::DrawUI();

        ImGui::Begin("System Controls");
        
        //da toggle din GUI la useDOD intre true si false
        ImGui::Checkbox("Use DOD)", &useDOD);
        
        int count = useDOD ? dodSystem.getCount() : 0; // oopSystem.getCount()
        ImGui::Text("Total Balls: %d", count);
        
        if (ImGui::Button("Clear Toate Bilele")) {
            if (useDOD) dodSystem.Despawn(0);
            // else oopSystem.Despawn(0);
        }
        
        ImGui::End();
        rlImGuiEnd(); 
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}

// Spawneaza o minge cu stats-uri random la pozitia mouse-ului 
void check_mouse(bool useDOD, DODSystem& dodSystem /*, OOPSystem& oopSystem*/) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int mX = GetMouseX();
        int mY = GetMouseY();
        int vX = (rand() % 10) - 5;
        int vY = (rand() % 10) - 5;
        int radius = 5;
        Color c = { 
            (unsigned char)(rand() % 256), 
            (unsigned char)(rand() % 256), 
            (unsigned char)(rand() % 256), 
            255 
        };
        
        if (useDOD) {
            dodSystem.SpawnOne(mX, mY, vX, vY, radius, c);
        } else {
            // oopSystem.SpawnOne(mX, mY, vX, vY, radius, c);
        }
    }
}

//goleste vectorii de grid
void clear_grid(std::vector<std::vector<int>>& grid) {
    for (int i = 0; i < (int)grid.size(); i++) {
        grid[i].clear();
    }
}

//face miscarea bilelor si dupa updateaza gridul
void update_and_populate_grid(bool useDOD, DODSystem& dodSystem, /* OOPSystem& oopSystem, */ std::vector<std::vector<int>>& grid, int CellSize, int cols, int rows) {
    int currentCount = useDOD ? dodSystem.getCount() : 0; 
    
    for(int i = 0; i < currentCount; i++) {
        int x = 0, y = 0;
        int raza = 5;

        //miscare in functie de metoda selectata
        if (useDOD) {
            dodSystem.MoveDOD(i, x, y);
            raza = dodSystem.getRadius(i);
        } else {
            // oopSystem.MoveOOP(i, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, x, y);
            // raza = oopSystem.getRadius(i);
        }

        //calcul celule din grid atinse
        int minCol = (x - raza) / CellSize;
        int maxCol = (x + raza) / CellSize;
        int minRow = (y - raza) / CellSize;
        int maxRow = (y + raza) / CellSize;

        if (minCol < 0) minCol = 0;
        if (maxCol > cols - 1) maxCol = cols - 1;
        if (minRow < 0) minRow = 0;
        if (maxRow > rows - 1) maxRow = rows - 1;

        // Adaugare index in grid, in functie de celulele poe care le atinge
        for(int r = minRow; r <= maxRow; r++) {
            for(int c = minCol; c <= maxCol; c++) {
                int gridIndex = r * cols + c;
                grid[gridIndex].push_back(i);
            }
        }
    }
}
//verificare coliziuni luand ca input vectorul 1D al unei celule din grid
void resolve_cell_collisions(bool useDOD, const std::vector<int>& cellIndexes, DODSystem& dodSystem /*, OOPSystem& oopSystem*/) {
    int pInCell = cellIndexes.size();
    if (pInCell < 2) return; 

    for (int i = 0; i < pInCell - 1; i++) {
        for (int j = i + 1; j < pInCell; j++) {
            int id1 = cellIndexes[i];
            int id2 = cellIndexes[j];

            if (useDOD) {
                float dX = dodSystem.getPosX(id2) - dodSystem.getPosX(id1);
                float dY = dodSystem.getPosY(id2) - dodSystem.getPosY(id1);
                float distanceSq = dX * dX + dY * dY;
                float radiusSum = dodSystem.getRadius(id1) + dodSystem.getRadius(id2);
                float radiusSumSq = radiusSum * radiusSum;

                if (distanceSq < radiusSumSq && distanceSq > 0.0001f) {
                    int tempVx = dodSystem.getVelX(id1);
                    int tempVy = dodSystem.getVelY(id1);
                    dodSystem.setVelX(id1, dodSystem.getVelX(id2));
                    dodSystem.setVelY(id1, dodSystem.getVelY(id2));
                    dodSystem.setVelX(id2, tempVx);
                    dodSystem.setVelY(id2, tempVy);
                }
            } else {
                // LOGICA OOP
                
            }
        }
    }
}

//parcurge gridul si cheama functia de mai sus pt fiecare vector 1D
void resolve_all_collisions(bool useDOD, const std::vector<std::vector<int>>& grid, DODSystem& dodSystem /*, OOPSystem& oopSystem*/) {
    for (int g = 0; g < (int)grid.size(); g++) {
        resolve_cell_collisions(useDOD, grid[g], dodSystem);
    }
}

//Functie pt Draw
void draw_balls(bool useDOD, DODSystem& dodSystem /*, OOPSystem& oopSystem*/) {
    if (useDOD) {
        dodSystem.DrawRGB();
    } else {
        // oopSystem.DrawRGB();
    }
}