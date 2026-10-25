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
    void setColor(Color col) {
        color = col;
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
    // VELOCITY HEATMAP PENTRU OOP
    void UpdateVelocityHeatmap(int maxSpeed) {
        //maresc usor pragul maxim de referință (cu 15%) luat din sliderul de viteza maxima din main.cpp
        //din cauza ca sistemul meu poate genera, in unele cazuri, energie, 
        //iar in timp majoritatea bileleor vor ajunge la viteza maxima, ba chiar sa o depaseasca
        float effectiveMaxSpeed = (float)maxSpeed * 1.15f;

        // calculez patratul vitezei maxime efective
        float maxSpeedSq = static_cast<float>(effectiveMaxSpeed * effectiveMaxSpeed);
        if (maxSpeedSq < 0.0001f) maxSpeedSq = 1.0f; // Prevenim impartirea la 0

        int count = (int)bileArrayOOP.size();
        for (int i = 0; i < count; i++) {
            // calculez patratul vitezei ca sa nu folosesc sqrt()
            float vx = (float)bileArrayOOP[i].getVelX();
            float vy = (float)bileArrayOOP[i].getVelY();
            float speedSq = vx * vx + vy * vy;

            float t = speedSq / maxSpeedSq;
            if (t > 1.0f) t = 1.0f; // plafonez la 1.0 daca depaseste viteza maxima

            // generez un grad de culoare (de la Albastru - lent, spre Verde, pana la Roșu - rapid)
            unsigned char red = (unsigned char)(t * 255);
            unsigned char blue = (unsigned char)((1.0f - t) * 255);
            unsigned char green = (unsigned char)((1.0f - std::abs(t - 0.5f) * 2.0f) * 255);

            bileArrayOOP[i].setColor({ red, green, blue, 255 });
        }
    }
    
};