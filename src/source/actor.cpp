#include "actor.h"

void actor::make(int x, int y, int a, int ani[]){
    tileID=x, solidity=y, totalHealth=a, I=J=-1, reference=0, counter=0, anim=ani;
    health=totalHealth;
    clock=0;
    attackCharge=0;
    delay=20;
    scream=false;
    spriteRef=anim[0];

    sizeArray=0;
    while(anim[sizeArray]!=-1){
        sizeArray++;
    }
}
void actor::reset(){
    health=totalHealth;
    attackCharge=0;
    solidity=solidEnemy;
}
void actor::resetHealth(){
    health=3;
}
bool actor::getScream(){
    return scream;
}
void actor::setScream(bool val){
    scream=val;
}
int actor::getDelay(){
    return delay;
}
int actor::getClock(){
    return clock;
}
void actor::incClock(){
    if(I!=-1&&J!=-1){clock+=1;}
}
void actor::setDelay(int del){
    delay = del;
}
void actor::incCounter(){
        counter++;
        counter=counter%sizeArray;
        if(counter==0&&sizeArray>2){counter++;}
        spriteRef=anim[counter];

}
void actor::defaultSprite(){
        spriteRef=anim[0];
    }
void actor::setAnim(int ani[]){
    if (ani!=anim){
        counter=0;
        anim=ani;
        spriteRef=anim[0];

        sizeArray=0;
        while(anim[sizeArray]!=-1){
            sizeArray++;
        }
    }//end if
}
int actor::getI(){
    return I;
}
int actor::getJ(){
    return J;
}
void actor::setI(int x){
    I=x;
}
void actor::setJ(int x){
    J=x;
}
int actor::getHealth(){
    return health;
}
void actor::setHealth(int h){
    health=h;
}
void actor::takeHit(){
    if(health>0){health--;}
}
void actor::move(tile grid[][gridJ],int dir, int &newI, int &newJ){
    grid[I][J]=tileEmpty;

    switch(dir){
        case DOWN:
            I++;
            break;
        case UP:
            I--;
            break;
        case LEFT:
            J--;
            break;
        case RIGHT:
            J++;
            break;
    }
    //update the character location
    newI=I;
    newJ=J;
}

actor tileZombieA, tileZombieB,tileZombieC,tileZombieD,tileZombieE;