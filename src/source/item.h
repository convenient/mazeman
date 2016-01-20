#ifndef ITEM_H
#define ITEM_H

#include "tile.h"

class item: public tile{
    public:
        int I, J;
        bool status;
        int *anim;
        int counter;
        int sizeArray;

        void make(int x, int y, int ani[]);

        void incCounter();
        int getSpriteSize();
        void defaultSprite();
        void setAnim(int ani[]);
        int * getAnim();
        int getI();
        bool getStatus();
        void setStatus(bool val);
        int getJ();
        void setI(int x);
        void setJ(int x);
};

extern item tileEnd, tileRDoor,tileRDoorBeta, tileRedButton, tileRedButtonBeta, tileGDoor, tileGDoorBeta, tileGreenButton,tileGreenButtonBeta, tileBlueButton,
                tileBlueDoor, tileBlueButtonBeta, tileBlueDoorBeta, tileOrangeDoor, tileOrangeDoorBeta, tileOrangeButton,
                tileOrangeButtonBeta, tileGold;

#endif