#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include <vector>

class Bila{
    
    int xcoord;
    int ycoord;
    int xspeed;
    int yspeed;
    int width;
    public:
    Bila(){
        xcoord = rand()%801+400;
        ycoord = rand()%801+400;
        xspeed = rand()%6-3;
        yspeed = rand()%6-3;
        width = 20;
    }
    void moveBalls(){
        xcoord+=xspeed;
        ycoord+=yspeed;
        if(ycoord > 1600 || ycoord < 0){
            yspeed*=-1;
        }
        if(xcoord > 1600 || xcoord < 0){
            xspeed*=-1;
        }
    }
    int getx(){
        return xcoord;
    }
    int gety(){
        return ycoord;
    }
    int getwidth(){
        return width;
    }
};

int main(void) 
{
    srand(time(0));
    Color green = {20, 160, 133, 255};

    InitWindow(1600, 1600, "Example");
    SetTargetFPS(60);
<<<<<<< Updated upstream
    Bila bila1;
    Bila bila2;
    Bila bila3;
    Bila bila4;
    Bila bila5;
    Bila bila6;
    Bila bila7;
    Bila bila8;
    Bila bila9;
    Bila bila10;
=======
    std::vector<Bila> bileVector;
    for (int i = 0; i < 10000; i++){

        bileVector.push_back(Bila());
    }
>>>>>>> Stashed changes
    // Game Loop
    while (WindowShouldClose() == false) 
    {
        BeginDrawing();
        ClearBackground(green);
<<<<<<< Updated upstream
        bila1.moveBalls();
        bila2.moveBalls();
        bila3.moveBalls();
        bila4.moveBalls();
        bila5.moveBalls();
        bila6.moveBalls();
        bila7.moveBalls();
        bila8.moveBalls();
        bila9.moveBalls();
        bila10.moveBalls();
        DrawCircle(bila1.getx(), bila1.gety(), bila1.getwidth(), WHITE);
        DrawCircle(bila2.getx(), bila2.gety(), bila2.getwidth(), WHITE);
        DrawCircle(bila3.getx(), bila3.gety(), bila3.getwidth(), WHITE);
        DrawCircle(bila4.getx(), bila4.gety(), bila4.getwidth(), WHITE);
        DrawCircle(bila5.getx(), bila5.gety(), bila5.getwidth(), WHITE);
        DrawCircle(bila6.getx(), bila6.gety(), bila6.getwidth(), WHITE);
        DrawCircle(bila7.getx(), bila7.gety(), bila7.getwidth(), WHITE);
        DrawCircle(bila8.getx(), bila8.gety(), bila8.getwidth(), WHITE);
        DrawCircle(bila9.getx(), bila9.gety(), bila9.getwidth(), WHITE);
        DrawCircle(bila10.getx(), bila10.gety(), bila10.getwidth(), WHITE);
=======
        for(auto& bila : bileVector){
            DrawCircle(bila.getx(), bila.gety(), bila.getwidth(), WHITE);
            bila.moveBalls();
        }
>>>>>>> Stashed changes
        EndDrawing();
    }

    CloseWindow();
    return 0;
}