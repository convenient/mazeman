#ifndef ACTOR_H
#define ACTOR_H

#include "tile.h"
#include "constants.h"

class actor: public tile{
    public:
    int health;
    int I,J;
    int *anim;
    int counter;
    int sizeArray;
    int clock;
    int delay;
    int attackCharge;
    bool scream;
    int totalHealth;

    void make(int x, int y, int a, int ani[]);
    void reset();
    void resetHealth();
    bool getScream();
    void setScream(bool val);
    int getDelay();
    int getClock();
    void incClock();
    void setDelay(int del);
    void incCounter();
    void defaultSprite();
    void setAnim(int ani[]);
    int getI();
    int getJ();
    void setI(int x);
    void setJ(int x);
    int getHealth();
    void setHealth(int h);
    void takeHit();
    void move(tile grid[][gridJ],int dir, int &newI, int &newJ);
};

extern actor tileZombieA, tileZombieB,tileZombieC,tileZombieD,tileZombieE;

#endif