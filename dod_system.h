#pragma once
#include <vector>
#include <cstdlib>
#include "raylib.h"
#include "config.h"
/// @brief metoda DOD de rezolvare
//FUNCTII: SpawnOne(), Spawn(), Despawn(), Move(), ResolveCollisions(), DrawRGB(), DrawOne()
// Getteri pentru toate proprietatile|Setteri pentru toate proprietatile

struct DODSystem {
    // Structuri de date
    std::vector<int> posX;
    std::vector<int> posY;
    std::vector<int> velX;
    std::vector<int> velY;
    std::vector<int> radius;
    std::vector<Color> colors;

    // SPAWN ONE FUNCTION
    void SpawnOne(int posX, int posY, int velX, int velY, int radius, Color color) {
        this->posX.push_back(posX);
        this->posY.push_back(posY);
        this->velX.push_back(velX);
        this->velY.push_back(velY);
        this->radius.push_back(radius);
        this->colors.push_back(color);
    }

    // SPAWN FUCNTION
    void Spawn(int newCount, float screenWidth, float screenHeight, int randDim, int randVel) {
        int oldSize;
        if (posX.empty()) oldSize = 0;
        else { 
            oldSize = (int)posX.size();
        }
        posX.resize(newCount);
        posY.resize(newCount);
        velX.resize(newCount);
        velY.resize(newCount);
        radius.resize(newCount);
        colors.resize(newCount);

        for (int i = oldSize; i < newCount; i++) {
            posX[i] = (float)(rand() % (int)screenWidth);
            posY[i] = (float)(rand() % (int)screenHeight);
            
            // Viteze aleatorii mici
            velX[i] = ((float)(rand() % randVel) - randVel/2) / 30.0f;
            velY[i] = ((float)(rand() % randVel) - randVel/2) / 30.0f;
            // % randVel ca sa ia valori intre 0 si randVel-1;
            // - randVel/2 ca sa ia valori intre -randVel/2 si randVel/2-1;
            // / 30.0f ca sa le facem mai mici, intre -randVel/30 si randVel/30-1/30;
            radius[i] = 5.0f; // Raza cercului
            
            //RGB aleatorii pentru culoare
            colors[i] = { (unsigned char)(rand() % 256), 
                          (unsigned char)(rand() % 256), 
                          (unsigned char)(rand() % 256), 
                          255 };
        }
    }

    // REMOVE FUCNTION
    // metoda de a sterge un element din vectori la un index dat
    void RemoveParticleAt(int index) {
        if (posX.empty()) return;

        int lastIndex = (int)posX.size() - 1;

        //verific sa fie index VALID
        if(index < 0 || index > lastIndex) return;

        //scriu elementul de pe ultima pos peste elementul de sters
        posX[index] = posX[lastIndex];
        posY[index] = posY[lastIndex];
        velX[index] = velX[lastIndex];
        velY[index] = velY[lastIndex];
        radius[index] = radius[lastIndex];
        colors[index] = colors[lastIndex];

        //scot ultimul element din vectori
        posX.pop_back();
        posY.pop_back();
        velX.pop_back();
        velY.pop_back();
        radius.pop_back();
        colors.pop_back();
        //IMPORTANT!! dupa pop_back() dimensiunea vectorilor scade cu 1
    }

    // DESPAWN FUCNTION
    void Despawn(int newCount) {
        int oldSize = (int)posX.size();

        if (newCount < oldSize) {
            for (int i = 0; i < oldSize - newCount; i++) {
                if(posX.empty()) break;
                // aleg random la ce index dau delete pastrand restul de elemente
                RemoveParticleAt(rand() % posX.size());
            }
        }
    }


    // MOVE FUNCTION
    void Move(int newCount) {
        int count = (int)posX.size();

        for(int i = 0; i < count; i++) {
            posX[i] += velX[i];
            posY[i] += velY[i];

            // COLIZIUNE cu borderul windowului
            if (posX[i] - radius[i] < 0) { posX[i] = radius[i]; velX[i] *= -1; }
            if (posX[i] + radius[i] > Config::SCREEN_WIDTH) { posX[i] = Config::SCREEN_WIDTH - radius[i]; velX[i] *= -1; }
            if (posY[i] - radius[i] < 0) { posY[i] = radius[i]; velY[i] *= -1; }
            if (posY[i] + radius[i] > Config::SCREEN_HEIGHT) { posY[i] = Config::SCREEN_HEIGHT - radius[i]; velY[i] *= -1; }
        }
    }

    // MOVEDOD FUNCTION
    void MoveDOD(int index, int &newX, int &newY){
        if (index < 0 || index >= (int)posX.size()) return; // verific daca indexul este valid
        posX[index] += velX[index];
        posY[index] += velY[index];

        // COLIZIUNE cu borderul windowului
        if (posX[index] - radius[index] < 0) { posX[index] = radius[index]; velX[index] *= -1; }
        if (posX[index] + radius[index] > Config::SCREEN_WIDTH) { posX[index] = Config::SCREEN_WIDTH - radius[index]; velX[index] *= -1; }
        if (posY[index] - radius[index] < 0) { posY[index] = radius[index]; velY[index] *= -1; }
        if (posY[index] + radius[index] > Config::SCREEN_HEIGHT) { posY[index] = Config::SCREEN_HEIGHT - radius[index]; velY[index] *= -1; }

        newX = posX[index];
        newY = posY[index];
    }
    //COLIZIUNE FUCNTION 
    // pentru valoare 100.000 o sa mearg prost pentru o verificare N^2
    void ResolveCollisions(){
        int count = (int)posX.size();
        for (int i = 0; i < count - 1; i++) {
            for(int j = i + 1; j < count; j++)
            {
                //Calcul distanta euclidiana intre doua cercuri
                float dX = posX[j] - posX[i];
                float dY = posY[j] - posY[i];
                float distanceSq = dX * dX + dY * dY;
                float radiusSum = radius[i] + radius[j];
                float radiusSumSq = radiusSum * radiusSum;
    
                //verific patratul distantelor ca e mai cost efficient decat sa fac sqrt
                //fac verificarea > 0.0001f ca sa nu am eroare la impartirea la 0
                if(distanceSq < radiusSumSq && distanceSq > 0.0001f) {
                    //coliziune cu efect elastic
                    float tempVx = velX[i];
                    float tempVy = velY[i];
                    // schimbam vitezele intre cele doua cercuri
                    velX[i] = velX[j];
                    velY[i] = velY[j];
                    velX[j] = tempVx;
                    velY[j] = tempVy;
                }

            }
        }
    }

    // DRAWRGB FUNCTION
    void DrawRGB(const Texture2D& tex) {
        const Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
        int count = (int)posX.size();
        for (int i = 0; i < count; i++) {
            float r = (float)radius[i];
            Rectangle dst = { posX[i] - r, posY[i] - r, 2 * r, 2 * r };
            DrawTexturePro(tex, src, dst, { 0, 0 }, 0.0f, colors[i]);
    }
}

    // DRAWONE FUNCTION
    void DrawOne(int index) {
        if (index < 0 || index >= (int)posX.size()) return; // verific daca indexul este valid
        //DrawCircle((int)posX[index], (int)posY[index], radius[index], colors[index]);
        DrawPixel((int)posX[index], (int)posY[index], {255, 255, 255, 255});
    }


    int getPosX(int index) {
        // verific daca indexul este valid
        if (index < 0 || index >= (int)posX.size()) return -1; 
        return posX[index];
    }
    
    int getPosY(int index) {
        // verific daca indexul este valid
        if (index < 0 || index >= (int)posY.size()) return -1; 
        return posY[index];
    }

    int getVelX(int index) {
        // verific daca indexul este valid
        if (index < 0 || index >= (int)velX.size()) return -1; 
        return velX[index];
    }

        int getVelY(int index) {
        // verific daca indexul este valid
        if (index < 0 || index >= (int)velY.size()) return -1; 
        return velY[index];
    }

    int getRadius(int index) {
        // verific daca indexul este valid
        if (index < 0 || index >= (int)radius.size()) return -1; 
        return radius[index];
    }

    int getColorR(int index) {
        // verific daca indexul este valid
        if (index < 0 || index >= (int)colors.size()) return -1; 
        return colors[index].r;
    }
    int getColorG(int index) {
        // verific daca indexul este valid
        if (index < 0 || index >= (int)colors.size()) return -1; 
        return colors[index].g;
    }
    int getColorB(int index) {
        // verific daca indexul este valid
        if (index < 0 || index >= (int)colors.size()) return -1; 
        return colors[index].b;
    }
    int getCount(){
        if(posX.empty()) return 0;
        else return (int)posX.size();
    }
    void setPosX(int index, int value) {
        if (index < 0 || index >= (int)posX.size()) return; 
        posX[index] = value;
    }
    void setPosY(int index, int value) {
        if (index < 0 || index >= (int)posY.size()) return; 
        posY[index] = value;
    }
    void setVelX(int index, int value) {
        if (index < 0 || index >= (int)velX.size()) return; 
        velX[index] = value;
    }
    void setVelY(int index, int value) {
        if (index < 0 || index >= (int)velY.size()) return; 
        velY[index] = value;
    }
    void setRadius(int index, int value) {
        if (index < 0 || index >= (int)radius.size()) return; 
        radius[index] = value;
    }
    void setColor(int index, Color value) {
        if (index < 0 || index >= (int)colors.size()) return; 
        colors[index] = value;
    }
};