#include <vector>
#include <cmath>
#include <iostream>

#include "config.h"
#include "miscarerandom.cpp"
#include "dod_system.h"

// vezi dod_systems functie
void ObjectCollision(int count, int cell_array[count], int index) {

    for (int i = 0; i < count - 1; i++) {
        for(int j = i + 1; j < count; j++) {
            //Calcul distanta euclidiana intre doua cercuri

            // oop - wip

            // dod
            float dX = getPosX(cell_array[j]) - getPosX(cell_array[i]);
            float dY = getPosY(cell_array[j]) - getPosY(cell_array[i]);
            float distanceSq = dX * dX + dY * dY;
            float radiusSum = getRadius(cell_array[i]) + getRadius(cell_array[j]);
            float radiusSumSq = radiusSum * radiusSum;
    
            //verific patratul distantelor ca e mai cost efficient decat sa fac sqrt
            //fac verificarea > 0.0001f ca sa nu am eroare la impartirea la 0
            if(distanceSq < radiusSumSq && distanceSq > 0.0001f) {
                //coliziune cu efect elastic
                float tempVx = getVelX(cell_array[i]);
                float tempVy = getVelY(cell_array[i]);
                // schimbam vitezele intre cele doua cercuri
                setVelX(cell_array[i], getVelX(cell_array[j]));
                setVelY(cell_array[i], getVelX(cell_array[j]));
                setVelX(cell_array[j], tempVx);
                setVelY(cell_array[j], tempVy);
            }
        }
    }
}


int test(void)
{
    srand((unsigned)time(0));
 
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Config::WINDOW_TITLE);
    SetTargetFPS(60);
 
    std::vector<Bila> bileVector;
    bileVector.reserve(BALL_COUNT);
    for (int i = 0; i < BALL_COUNT; i++) bileVector.push_back(Bila());
 
    while (WindowShouldClose() == false)
    {
        for (auto& bila : bileVector) bila.moveBalls();
        ResolveCollision(bileVector);
 
        BeginDrawing();
        ClearBackground(Config::BACKGROUND);
        for (auto& bila : bileVector)
            DrawCircleV({bila.getx(), bila.gety()}, bila.getwidth(), WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }
 
    CloseWindow();
    return 0;
}