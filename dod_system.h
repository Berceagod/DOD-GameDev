#pragma once
#include <vector>
#include <cstdlib>
#include "raylib.h"

/// @brief metoda DOD de rezolvare
// FUNCTII: Spawn(), Update(), ResolveCollisions(), Draw()

struct DODSystem {
    // Structuri de date
    std::vector<int> posX;
    std::vector<int> posY;
    std::vector<int> velX;
    std::vector<int> velY;
    std::vector<int> radius;
    std::vector<Color> colors;

    // SPAWN FUCNTION 
    void Spawn(int count, float screenWidth, float screenHeight) {
        posX.resize(count);
        posY.resize(count);
        velX.resize(count);
        velY.resize(count);
        radius.resize(count);
        colors.resize(count);

        for (int i = 0; i < count; i++) {
            posX[i] = (float)(rand() % (int)screenWidth);
            posY[i] = (float)(rand() % (int)screenHeight);
            
            // Viteze aleatorii mici
            velX[i] = ((float)(rand() % 200) - 100) / 30.0f;
            velY[i] = ((float)(rand() % 200) - 100) / 30.0f;
            // % 200 ca sa ia valori intre 0 si 199;
            // - 100 ca sa ia valori intre -100 si 99;
            // / 30.0f ca sa le facem mai mici, intre -3.33 si 3.3
            
            radius[i] = 5.0f; // Raza cercului
            
            //RGB aleatorii pentru culoare
            colors[i] = { (unsigned char)(rand() % 256), 
                          (unsigned char)(rand() % 256), 
                          (unsigned char)(rand() % 256), 
                          255 };
        }
    }

    // UPDATE FUNCTION
    void Update(float screenWidth, float screenHeight) {
        int count = posX.size(); // count devine size de posX
        for(int i = 0; i < count; i++) {
            posX[i] += velX[i];
            posY[i] += velY[i];

            // COLIZIUNE cu borderul windowului
            if (posX[i] - radius[i] < 0) { posX[i] = radius[i]; velX[i] *= -1; }
            if (posX[i] + radius[i] > screenWidth) { posX[i] = screenWidth - radius[i]; velX[i] *= -1; }
            if (posY[i] - radius[i] < 0) { posY[i] = radius[i]; velY[i] *= -1; }
            if (posY[i] + radius[i] > screenHeight) { posY[i] = screenHeight - radius[i]; velY[i] *= -1; }
        }
    }

    //COLIZIUNE FUCNTION 
    // pentru valoare 100.000 o sa mearg prost pentru o verificare N^2
    void ResolveCollisions(){
        int count = posX.size();
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
                if(distanceSq < radiusSumSq && distanceSq > 0.0001f){
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

    // DRAW FUNCTION
    void Draw() {
        int count = posX.size();
        for (int i = 0; i < count; i++) {
            // Fac casting la int pentru ca raylib foloseste int pentru pozitiile de desenat(nuamr intreg de pixeli)
            DrawCircle((int)posX[i], (int)posY[i], radius[i], colors[i]);
        }
    }
}