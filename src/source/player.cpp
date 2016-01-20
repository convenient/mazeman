#include "player.h"
#include "sound.h"

void player::make(int x, int y, int ani[], int mode){
    tileID=x, solidity=y, I=-1, J=-1,totalmoves=0, reference=0, anim=ani;

    if(mode==HARD){totalHealth=3;}
    if(mode==NORMAL){totalHealth=6;}
    if(mode==EASY){totalHealth=9;}

    health=totalHealth;
    attacking=false;
    counter=0;
    spriteRef=anim[0];

    sizeArray=0;
    while(anim[sizeArray]!=-1){
        sizeArray++;
    }

}
void player::reset(){
    I=-1, J=-1,totalmoves=0, reference=0;

    attacking=false;
    counter=0;
    spriteRef=anim[0];
    health=totalHealth;
    sizeArray=0;
    while(anim[sizeArray]!=-1){
        sizeArray++;
    }
}
int player::getTotalHealth(){
    return totalHealth;
}
void player::incClock(){
    int breakLimit=15;
    int limit=20;

    if(attackCharge>breakLimit){
        attacking=false;
    }
    if(attackCharge<limit){attackCharge++;}

    clock+=1;
}
void player::attackUsed(){
    attackCharge=0;
    attacking=true;
}
bool player::getAttacking(){
    return attacking;
}
void player::cancelAttacking(){
    attacking=false;
}
int player::attackReady(){
    return attackCharge;
}
void player::incCounter(){
    counter++;
    counter=counter%sizeArray;
    if(counter==0&&sizeArray>2){counter++;}
    spriteRef=anim[counter];

}
int player::getSteps(){
    return totalmoves;
}
void player::takeHit(){
    if(health>0){health--;}
    mmEffectEx(&hurt);
}
void player::defaultSprite(){
    spriteRef=anim[0];
}
void player::setAnim(int ani[]){
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
void player::resetSteps(){
    totalmoves=0;
}
void player::move(tile grid[][gridJ],int dir, int &newI, int &newJ){
    grid[I][J]=tileEmpty;
    totalmoves++;
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
player tileMazeMan;