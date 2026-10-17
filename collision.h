#include <vector>
#include <cmath>
#include <iostream>

#include "config.h"
#include "dod_system.h"

// vezi dod_systems functie
inline void ObjectCollision(bool useDOD, const std::vector<int>& cell_array, DODSystem& dodSystem /*, OOPSystem& oopSystem*/) {
    int count = cell_array.size();
    if (count < 2) return;

    for (int i = 0; i < count - 1; i++) {
        for(int j = i + 1; j < count; j++) {
            //Calcul distanta euclidiana intre doua cercuri

            // oop - wip

            // dod
            float dX = dodSystem.getPosX(cell_array[j]) - dodSystem.getPosX(cell_array[i]);
            float dY = dodSystem.getPosY(cell_array[j]) - dodSystem.getPosY(cell_array[i]);
            float distanceSq = dX * dX + dY * dY;
            float radiusSum = dodSystem.getRadius(cell_array[i]) + dodSystem.getRadius(cell_array[j]);
            float radiusSumSq = radiusSum * radiusSum;
    
            //verific patratul distantelor ca e mai cost efficient decat sa fac sqrt
            //fac verificarea > 0.0001f ca sa nu am eroare la impartirea la 0
            if(distanceSq < radiusSumSq && distanceSq > 0.0001f) {
                //coliziune cu efect elastic
                float tempVx = dodSystem.getVelX(cell_array[i]);
                float tempVy = dodSystem.getVelY(cell_array[i]);
                // schimbam vitezele intre cele doua cercuri
                dodSystem.setVelX(cell_array[i], dodSystem.getVelX(cell_array[j]));
                dodSystem.setVelY(cell_array[i], dodSystem.getVelY(cell_array[j]));
                dodSystem.setVelX(cell_array[j], tempVx);
                dodSystem.setVelY(cell_array[j], tempVy);
            }
        }
    }
}
