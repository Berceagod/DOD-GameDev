#pragma once
#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "config.h"
#include <cmath>

class Bila {
    int posX;
    int posY;
    int velX;
    int velY;
    int radius;
    Color color;
    public:
    Bila(int r = (int)Config::BALL_RADIUS, int xpos = (int)Config::BALL_START_X, int ypos = (int)Config::BALL_START_Y, int xspeed = (int)Config::BALL_SPEED, int yspeed = (int)Config::BALL_SPEED, Color col = {255,255,255,255}){
            radius = r;
            posX = xpos;
            posY = ypos;
            velX = xspeed;
            velY = yspeed;
            color = col;
    }
    int getX(){
        return posX;
    }
    int getY(){
        return posY;
    }
    int getRadius(){
        return radius;
    }
    int getVelX() {
        return velX;
    }
    int getVelY() {
        return velY;
    }
    Color getColor(){
        return color;
    }
    void setPosX(int x) {
        posX = x;
    }
    void setPosY(int y) {
        posY = y;
    }
    void setVelX(int xsp) {
        velX = xsp;
    }
    void setVelY(int ysp) {
        velY = ysp;
    }
    void moveBall(){
        posX+=velX;
        posY+=velY;
        if (posX < radius) { 
            posX = radius;
            velX = std::fabs(velX); 
        }
        if (posX > Config::SCREEN_WIDTH - radius) { 
            posX = Config::SCREEN_WIDTH - radius;  
            velX = -std::fabs(velX); 
        }
        if (posY < radius) { 
            posY = radius;
            velY =  std::fabs(velY); 
        }
        if (posY > Config::SCREEN_HEIGHT - radius) { 
            posY = Config::SCREEN_HEIGHT - radius;  
            velY = -std::fabs(velY); 
        }
    }

};

struct OOPSystem{

    std::vector<Bila> bileArrayOOP;
    void SpawnOne(int posX, int posY, int velX, int velY, int radius, Color color){
        bileArrayOOP.push_back(Bila(radius, posX, posY, velX, velY, color));
    }
    void Despawn(int newCount){
        int oldCount = (int)bileArrayOOP.size();
        if( newCount < oldCount){
            for (int i = 0; i < oldCount - newCount; i++) {
                if(bileArrayOOP.empty()) break;
                bileArrayOOP.erase(bileArrayOOP.begin() + rand() % bileArrayOOP.size());
            }
        }
    }
    void moveOOP(int i){
        bileArrayOOP[i].moveBall();
    }
    void getBallData(int i, int &x, int &y, int &r){
        x = bileArrayOOP[i].getX();
        y = bileArrayOOP[i].getY();
        r = bileArrayOOP[i].getRadius();    
    }
    int getCount(){
        return bileArrayOOP.size();
    }
    void DrawRGB(const Texture2D& tex) {
        const Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
        int count = (int)bileArrayOOP.size();
        for (int i = 0; i < count; i++) {
            float r = (float)bileArrayOOP[i].getRadius();
            Rectangle dst = {bileArrayOOP[i].getX() - r, bileArrayOOP[i].getY() - r, 2 * r, 2 * r };
            DrawTexturePro(tex, src, dst, { 0, 0 }, 0.0f, bileArrayOOP[i].getColor());
    }
}
    
};