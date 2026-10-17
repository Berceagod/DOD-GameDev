#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "config.h"

const int BALL_COUNT = 50;
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
        width = Config::BALL_RADIUS;
        xcoord = Config::BALL_START_X;
        ycoord = Config::BALL_START_Y;
        float angle = ((float)rand() / RAND_MAX) * 6.2831853f;
        float speed = Config::BALL_SPEED;
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
        if (xcoord > Config::SCREEN_WIDTH - width) { 
            xcoord = Config::SCREEN_WIDTH - width;  
            xspeed = -std::fabs(xspeed); 
        }
        if (ycoord < width) { 
            ycoord = width;
            yspeed =  std::fabs(yspeed); 
        }
        if (ycoord > Config::SCREEN_HEIGHT - width) { 
            ycoord = Config::SCREEN_HEIGHT - width;  
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

int test2(void) 
{
    srand(time(0));

    InitWindow(1600, 1600, Config::WINDOW_TITLE);
    SetTargetFPS(Config::TARGET_FPS);

    std::vector<Bila> bileVector;
    for (int i = 0; i < 10000; i++){

        bileVector.push_back(Bila());
    }
    // Game Loop
    while (WindowShouldClose() == false) 
    {
        BeginDrawing();
        ClearBackground(Config::BACKGROUND);
        for(auto& bila : bileVector){
            DrawCircle(bila.getx(), bila.gety(), bila.getwidth(), WHITE);
            bila.moveBalls();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}