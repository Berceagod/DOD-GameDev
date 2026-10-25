#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "imgui.h"
#include "rlImGui.h"
#include "config.h"
#include "benchmark.h"
#include "dod_system.h"
#include "collision.h"
#include "oop_system.h"

void check_mouse(bool useDOD, DODSystem& dodSystem,OOPSystem& oopSystem,int spawnCount, float mouseRadius, bool magnetMode, bool& rightMouseActive);
//TODO sa fie gen un singur vector, si sa apelezi cu index, in loc sa fie vector de vectori.
void clear_grid(std::vector<std::vector<int>>& grid);
void update_and_populate_grid(bool useDOD, DODSystem& dodSystem,OOPSystem& oopSystem, std::vector<std::vector<int>>& grid, int CellSize, int cols, int rows);
void resolve_all_collisions(bool useDOD, const std::vector<std::vector<int>>& grid, DODSystem& dodSystem, OOPSystem& oopSystem);
void draw_balls(bool useDOD, DODSystem& dodSystem, const Texture2D& ballTex, OOPSystem& oopSystem);
void apply_repulsion_logic(bool useDOD, DODSystem& dodSystem, OOPSystem& oopSystem, int mX, int mY, float mouseRadius);
void apply_magnetism_logic(bool useDOD, DODSystem& dodSystem, OOPSystem& oopSystem, int mX, int mY, float mouseRadius);


//ball speed
int maxSpeed = 5;
int minSpeed = 2;
int radius = 3;

int main(void)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Config::WINDOW_TITLE);


const int TEX = 64;  // rezoluție mare, ca să arate bine la scalare
Image img = GenImageColor(TEX, TEX, BLANK);
ImageDrawCircle(&img, TEX / 2, TEX / 2, TEX / 2 - 1, WHITE);
Texture2D ballTex = LoadTextureFromImage(img);
UnloadImage(img);                                  // imaginea din RAM nu mai trebuie
SetTextureFilter(ballTex, TEXTURE_FILTER_BILINEAR); // margini mai netede la scalare


    SetTargetFPS(Config::TARGET_FPS);
    rlImGuiSetup(true);
    
    // Variabila care controleaza daca se foloseste DOD sau OOP
    bool useDOD = false; 

    // variabila pentru heatmap
    bool enableHeatmap = false;

    // variabila de toggle pt repulse/magnet (click dreapta)
    bool magnetMode = false;

    // toogle pentru mentinerea activa a functiei de magnet/repulsie
    bool rightMouseActive = false;

    //numarul de  bile spawante per click
    int spawnCount = 10;
    //raza de spawn a bilelor
    float mouseBrushRadius = 50.0f;

    // Initializare Grid
    int CellSize = 7;
    std::vector<std::vector<int>> grid; 
    int cols = Config::SCREEN_WIDTH / CellSize;
    int rows = Config::SCREEN_HEIGHT / CellSize;
    if (Config::SCREEN_WIDTH % CellSize != 0) cols++;
    if (Config::SCREEN_HEIGHT % CellSize != 0) rows++;
    grid.resize(cols * rows);

    // Initializare Sisteme
    DODSystem dodSystem;
    OOPSystem oopSystem;

    while (!WindowShouldClose())
    {
        Benchmark::Begin();

        // spawn bile
        check_mouse(useDOD, dodSystem, oopSystem,spawnCount, mouseBrushRadius, magnetMode, rightMouseActive);

        //golire grid
        clear_grid(grid);

        //miscare si updatare grid
        update_and_populate_grid(useDOD, dodSystem,oopSystem, grid, CellSize, cols, rows);

        if (enableHeatmap) {
            if (useDOD) {
                dodSystem.UpdateVelocityHeatmap(maxSpeed);
            } else {
                oopSystem.UpdateVelocityHeatmap(maxSpeed);
            }
        }

        //coliziune
        resolve_all_collisions(useDOD, grid, dodSystem, oopSystem);

        //draw
        BeginDrawing();
        
        

        ClearBackground(Config::BACKGROUND);
        draw_balls(useDOD, dodSystem, ballTex,oopSystem);
        //deseneaza brushul
        DrawCircle(GetMouseX(), GetMouseY(), mouseBrushRadius, { 255, 255, 255, 50 });
        DrawCircleLines(GetMouseX(), GetMouseY(), mouseBrushRadius, { 255, 255, 255, 150 }); // Un mic contur
        
        Benchmark::End();

        // ---- GUI ----
        rlImGuiBegin();
        Benchmark::DrawUI();

        ImGui::Begin("System Controls");
        
        //da toggle din GUI la useDOD intre true si false
        ImGui::Checkbox("Use DOD)", &useDOD);

        //da toggle din GUI la Velocity Heatmap
        ImGui::Checkbox("Velocity Heatmap", &enableHeatmap);


        ImGui::Separator();
        ImGui::SliderInt("Numar Bile / Click", &spawnCount, 1, 10000);
        ImGui::SliderFloat("Mouse Size", &mouseBrushRadius, 10.0f, 300.0f);
        ImGui::Separator();
        ImGui::SliderInt("Viteza Maxima", &maxSpeed, minSpeed, 30);
        ImGui::SliderInt("Raza Bilelor", &radius, 1, 30);
        ImGui::Separator();
        ImGui::Checkbox("Repel / Magnet mode", &magnetMode);
        ImGui::Separator();
        int count = useDOD ? dodSystem.getCount() : oopSystem.getCount();
        ImGui::Text("Total Balls: %d", count);
        
        if (ImGui::Button("Clear Toate Bilele")) {
            if (useDOD) dodSystem.Despawn(0);
            else oopSystem.Despawn(0);
        }
        
        ImGui::End();
        rlImGuiEnd(); 
        EndDrawing();
    }

    rlImGuiShutdown();
    UnloadTexture(ballTex);
    CloseWindow();
    return 0;
}

// Spawneaza o minge cu stats-uri random la pozitia mouse-ului 
void check_mouse(bool useDOD, DODSystem& dodSystem , OOPSystem& oopSystem,int spawnCount, float mouseRadius, bool magnetMode, bool& rightMouseActive) {
    //daca se da click pe slidere, nu se declanseaza spawnul
     if (ImGui::GetIO().WantCaptureMouse) return;

    int mX = GetMouseX();
    int mY = GetMouseY();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // int mX = GetMouseX();
        // int mY = GetMouseY();
        int rInt = (int)mouseRadius;

        // spawn balls
        for (int i = 0; i < spawnCount; i++) {
            
            int offsetX, offsetY;
            
            //generam puncte intr-un patrat pana nimerim unul in cerc
            do {
                offsetX = (rand() % (2 * rInt + 1)) - rInt;
                offsetY = (rand() % (2 * rInt + 1)) - rInt;
            } while (offsetX * offsetX + offsetY * offsetY > rInt * rInt);

            //adaugam offsetul generat la pozitia mouse-ului
            int finalX = mX + offsetX;
            int finalY = mY + offsetY;


            int vX = (rand() % (maxSpeed - minSpeed + 1)) + minSpeed;
            int vY = (rand() % (maxSpeed - minSpeed + 1)) + minSpeed;

            // 50% sanse sa o ia in directia opusa (negativa)
            if (rand() % 2 == 0) vX = -vX;
            if (rand() % 2 == 0) vY = -vY;

            
            Color c = { 
                (unsigned char)(rand() % 256), 
                (unsigned char)(rand() % 256), 
                (unsigned char)(rand() % 256), 
                255 
            };
            
            if (useDOD) {
                dodSystem.SpawnOne(finalX, finalY, vX, vY, radius, c);
            } else {
                oopSystem.SpawnOne(finalX, finalY, vX, vY, radius, c);
            }
        }
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        rightMouseActive = !rightMouseActive;
    }
    
    if(rightMouseActive) {
        if (!magnetMode) {
            apply_repulsion_logic(useDOD, dodSystem, oopSystem, mX, mY, mouseRadius);
        } else {
            apply_magnetism_logic(useDOD, dodSystem, oopSystem, mX, mY, mouseRadius);
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
void update_and_populate_grid(bool useDOD, DODSystem& dodSystem, OOPSystem& oopSystem,  std::vector<std::vector<int>>& grid, int CellSize, int cols, int rows) {
    int currentCount = useDOD ? dodSystem.getCount() : oopSystem.getCount(); 
    
    for(int i = 0; i < currentCount; i++) {
        int x = 0, y = 0;
        int raza = 5;

        //miscare in functie de metoda selectata
        if (useDOD) {
            dodSystem.MoveDOD(i, x, y);
            raza = dodSystem.getRadius(i);
        } else {
            oopSystem.moveOOP(i);
            oopSystem.getBallData(i, x, y, raza);
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

//parcurge gridul si cheama functia de rezolvare a coliziunilor pt fiecare vector 1D
void resolve_all_collisions(bool useDOD, const std::vector<std::vector<int>>& grid, DODSystem& dodSystem, OOPSystem& oopSystem) {
    for (int g = 0; g < (int)grid.size(); g++) {
        ObjectCollision(useDOD, grid[g], dodSystem, oopSystem);
    }
}

//Functie pt Draw
void draw_balls(bool useDOD, DODSystem& dodSystem, const Texture2D& ballTex , OOPSystem& oopSystem) {
    if (useDOD) {
        dodSystem.DrawRGB(ballTex);
    } else {
        oopSystem.DrawRGB(ballTex);
    }
}

// aplica functionalitatea de repulsion / respingere a bilelor in afara razei cursorului
void apply_repulsion_logic(bool useDOD, DODSystem& dodSystem, OOPSystem& oopSystem, int mX, int mY, float mouseRadius) {
    int rInt = (int)mouseRadius;
    int count = useDOD ? dodSystem.getCount() : oopSystem.getCount();
    for (int i = 0; i < count; i++) {
        int x = 0, y = 0, r = 0;
        if (useDOD) {
            dodSystem.getBallData(i, x, y, r);
        } else {
            oopSystem.getBallData(i, x, y, r);
        }

        int dx = mX - x;
        int dy = mY - y;
        int distanceSq = dx * dx + dy * dy;
        int radiusSum = r + rInt;
        float relX = 50/(float)dx;
        float relY = 50/(float)dy;
        if(distanceSq <= radiusSum * radiusSum) {
            if(useDOD) {
                dodSystem.velX[i] =-1*(1+ relX);
                dodSystem.velY[i] =-1*(1+ relY);
            } else {
                oopSystem.bileArrayOOP[i].setVelX(-1*(1+ relX));
                oopSystem.bileArrayOOP[i].setVelY(-1*(1+ relY));
            }
        }
    }
}

// aplica functionalitatea de magnetism asupra bilelor in jurul cursorului
void apply_magnetism_logic(bool useDOD, DODSystem& dodSystem, OOPSystem& oopSystem, int mX, int mY, float mouseRadius) {
    
    int count = useDOD ? dodSystem.getCount() : oopSystem.getCount();
    float rangeSq = mouseRadius * mouseRadius; // raza la patrat, nu se face sqrt la fiecare bila

    for (int i = 0; i < count; i++) {
        int x = 0, y = 0, r = 0;
        if (useDOD) {
            dodSystem.getBallData(i, x, y, r);
        } else {
            oopSystem.getBallData(i, x, y, r);
        }

        //vector de la bila la cursor
        float dX = static_cast<float>(mX - x);
        float dY = static_cast<float>(mY - y);

        float distanceSq = dX * dX + dY * dY;

        // daca bila e in afara razei sau pe cursor, nu se face nimic
        if(distanceSq > rangeSq || distanceSq < 1.0f) continue;

        // viteze (velocity) noi, folosite pentru directia bilelor
        int vx = 0, vy = 0;

        if(distanceSq >= 4.0f) {
            float dist = std::sqrt(distanceSq);
            float speed = dist / 4.0f; // viteza depinde de distanta fata de cursor, daca sunt aproape incetinesc

            if (speed > static_cast<float>(maxSpeed)) {
                speed = static_cast<float>(maxSpeed);
            }
            if (speed < static_cast<float>(minSpeed)) {
                speed = static_cast<float>(minSpeed);
            }

            // directii normalizate pentru vitezele bilelor
            vx = static_cast<int>(std::round(dX / dist * speed));
            vy = static_cast<int>(std::round(dY / dist * speed));
        }

        // se suprascriu vitezele bilelor cu cele calculate mai sus
        if(useDOD) {
            dodSystem.velX[i] = vx;
            dodSystem.velY[i] = vy;
        } else {
            oopSystem.bileArrayOOP[i].setVelX(vx);
            oopSystem.bileArrayOOP[i].setVelY(vy);
        }
    }
}