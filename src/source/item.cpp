#include "item.h"

void item::make(int x, int y, int ani[]){
    tileID=x, solidity=y, I=-1, J=-1, reference=0, anim=ani;
    status=false;
    counter=0;
    spriteRef=anim[0];

    sizeArray=0;
    while(anim[sizeArray]!=-1){
        sizeArray++;
    }
}

void item::incCounter(){
    counter++;
    counter=counter%sizeArray;
    spriteRef=anim[counter];
}
int item::getSpriteSize(){
    return sizeArray;
}
void item::defaultSprite(){
    spriteRef=anim[0];
    counter=0;
}
void item::setAnim(int ani[]){
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
int * item::getAnim(){
    return anim;
}
int item::getI(){
    return I;
}
bool item::getStatus(){
    return status;
}
void item::setStatus(bool val){
    status=val;
}
int item::getJ(){
    return J;
}
void item::setI(int x){
    I=x;
}
void item::setJ(int x){
    J=x;
}

item tileEnd, tileRDoor,tileRDoorBeta, tileRedButton, tileRedButtonBeta, tileGDoor, tileGDoorBeta, tileGreenButton,tileGreenButtonBeta, tileBlueButton,
                tileBlueDoor, tileBlueButtonBeta, tileBlueDoorBeta, tileOrangeDoor, tileOrangeDoorBeta, tileOrangeButton,
                tileOrangeButtonBeta, tileGold;