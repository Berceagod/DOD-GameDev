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

    std::vector<Bila> bileVector;
    for (int i = 0; i < 10000; i++){

        bileVector.push_back(Bila());
    }
    // Game Loop
    while (WindowShouldClose() == false) 
    {
        BeginDrawing();
        ClearBackground(green);
        for(auto& bila : bileVector){
            DrawCircle(bila.getx(), bila.gety(), bila.getwidth(), WHITE);
            bila.moveBalls();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}