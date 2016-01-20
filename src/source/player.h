#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"

class player : public actor{
    public:
        int totalmoves;
        int *anim;
        int counter;

        int sizeArray;
        bool attacking;
        void make(int x, int y, int ani[], int mode);
        void reset();
        int getTotalHealth();
        void incClock();
        void attackUsed();
        bool getAttacking();
        void cancelAttacking();
        int attackReady();
        void incCounter();
        int getSteps();
        void takeHit();
        void defaultSprite();
        void setAnim(int ani[]);
        void resetSteps();
        void move(tile grid[][gridJ],int dir, int &newI, int &newJ);
};
extern player tileMazeMan;

#endif