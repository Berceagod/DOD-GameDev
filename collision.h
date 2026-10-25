#pragma once
#include <vector>
#include <cmath>
#include "dod_system.h"
#include "oop_system.h"

inline void ObjectCollision(bool useDOD, const std::vector<int>& cell_array, DODSystem& dodSystem, OOPSystem& oopSystem) {
    //if (!useDOD) return;

    int count = static_cast<int>(cell_array.size());
    if (count < 2) return;

    for (int i = 0; i < count - 1; i++) {
        for(int j = i + 1; j < count; j++) {
            int id1 = cell_array[i];
            int id2 = cell_array[j];

            float dX = useDOD ? static_cast<float>(dodSystem.posX[id2] - dodSystem.posX[id1]) : static_cast<float>(oopSystem.bileArrayOOP[id2].getX() - oopSystem.bileArrayOOP[id1].getX());
            float dY = useDOD ? static_cast<float>(dodSystem.posY[id2] - dodSystem.posY[id1]) : static_cast<float>(oopSystem.bileArrayOOP[id2].getY() - oopSystem.bileArrayOOP[id1].getY());
            float radiusSum = useDOD ? static_cast<float>(dodSystem.radius[id1] + dodSystem.radius[id2]) : static_cast<float>(oopSystem.bileArrayOOP[id2].getRadius() + oopSystem.bileArrayOOP[id1].getRadius());
            float distanceSq = dX * dX + dY * dY;

            if (distanceSq >= radiusSum * radiusSum) continue;

            float distance = std::sqrt(distanceSq);
            float nx = 1.0f;
            float ny = 0.0f;
            if (distance > 0.0001f) {
                nx = dX / distance;
                ny = dY / distance;
            }


            float halfOverlap = (radiusSum - distance) * 0.5f;
            int sepX = static_cast<int>(std::ceil(std::fabs(nx) * halfOverlap));
            int sepY = static_cast<int>(std::ceil(std::fabs(ny) * halfOverlap));
            if (nx < 0.0f) sepX = -sepX;
            if (ny < 0.0f) sepY = -sepY;
            if(useDOD){
            dodSystem.posX[id1] -= sepX;
            dodSystem.posY[id1] -= sepY;
            dodSystem.posX[id2] += sepX;
            dodSystem.posY[id2] += sepY;
            }else{
            oopSystem.bileArrayOOP[id1].setPosX(oopSystem.bileArrayOOP[id1].getX() - sepX);
            oopSystem.bileArrayOOP[id1].setPosY(oopSystem.bileArrayOOP[id1].getY() - sepY);
            oopSystem.bileArrayOOP[id2].setPosX(oopSystem.bileArrayOOP[id2].getX() + sepX); 
            oopSystem.bileArrayOOP[id2].setPosY(oopSystem.bileArrayOOP[id2].getY() + sepY); 
            }
            float relativeNormalVelocity = useDOD ?
                (dodSystem.velX[id2] - dodSystem.velX[id1]) * nx +
                (dodSystem.velY[id2] - dodSystem.velY[id1]) * ny 
                : 
                (oopSystem.bileArrayOOP[id2].getVelX() - oopSystem.bileArrayOOP[id1].getVelX()) * nx +
                (oopSystem.bileArrayOOP[id2].getVelY() - oopSystem.bileArrayOOP[id1].getVelY()) * ny;

            if (relativeNormalVelocity >= 0.0f) continue;

            int impulseX = static_cast<int>(std::round(relativeNormalVelocity * nx));
            int impulseY = static_cast<int>(std::round(relativeNormalVelocity * ny));
            if(useDOD){
            dodSystem.velX[id1] += impulseX;
            dodSystem.velY[id1] += impulseY;
            dodSystem.velX[id2] -= impulseX;
            dodSystem.velY[id2] -= impulseY;
            }else{
            oopSystem.bileArrayOOP[id1].setVelX(oopSystem.bileArrayOOP[id1].getVelX() + impulseX);
            oopSystem.bileArrayOOP[id1].setVelY(oopSystem.bileArrayOOP[id1].getVelY() + impulseY);
            oopSystem.bileArrayOOP[id2].setVelX(oopSystem.bileArrayOOP[id2].getVelX() - impulseX);
            oopSystem.bileArrayOOP[id2].setVelY(oopSystem.bileArrayOOP[id2].getVelY() - impulseY);
            }
        }
    }
}