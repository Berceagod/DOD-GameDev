#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include <vector>

const int WORLD_W = 1000;
const int WORLD_H = 1000;
const int BALL_COUNT = 10;
const float BALL_RADIUS = 50.0f;
const float MIN_SPEED = 1.0f;
const float MAX_SPEED = 3.0f;

class Bila{
    
    float xcoord;
    float ycoord;
    float xspeed;
    float yspeed;
    float width;

    public:
    Bila(){
        width = BALL_RADIUS;
        xcoord = width + (float)rand() / RAND_MAX * (WORLD_W - 2 * width);
        ycoord = width + (float)rand() / RAND_MAX * (WORLD_H - 2 * width);
        float angle = ((float)rand() / RAND_MAX) * 6.2831853f;
        float speed = MIN_SPEED + ((float)rand() / RAND_MAX) * (MAX_SPEED - MIN_SPEED);
        xspeed = std::cos(angle) * speed;
        yspeed = std::sin(angle) * speed;
        
    }
    void moveBalls(){
        xcoord+=xspeed;
        ycoord+=yspeed;
        if (xcoord < width) { 
            xcoord = width;
            xspeed = std::fabs(xspeed); 
        }
        if (xcoord > WORLD_W - width) { 
            xcoord = WORLD_W - width;  
            xspeed = -std::fabs(xspeed); 
        }
        if (ycoord < width) { 
            ycoord = width;
            yspeed =  std::fabs(yspeed); 
        }
        if (ycoord > WORLD_H - width) { 
            ycoord = WORLD_H - width;  
            yspeed = -std::fabs(yspeed); 
        }
    }
    float getx(){
        return xcoord;
    }
    float gety(){
        return ycoord;
    }
    float getwidth(){
        return width;
    }
    float getxspeed() {
        return xspeed;
    }
    float getyspeed() {
        return yspeed;
    }
    void setPos(float x, float y) {
        xcoord = x;
        ycoord = y;
    }
    void setSpeed(float xsp, float ysp) {
        xspeed = xsp;
        yspeed = ysp;
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