#include "input.h"
#include "sound.h"

#include <nds.h>
//#include <ctime>
#include <stdio.h>

#include "level.h"

void waitAtoContinue(){
    mmEffectEx(&keyPickupSound);
    secondDelay(1);
    iprintf("\x1b[21;1HPress 'A' to continue.");

    while(1){

        scanKeys();
        int held = keysHeld();

        if(held & KEY_A){
            break;
        }

        secondDelay(0.05);

    }//end-while

    mmEffectEx(&keyPickupSound);
    secondDelay(0.5);
}

void waitBtoReturn(){

    mmEffectEx(&ladderSound);
    iprintf("\x1b[21;1HPress 'B' to return.");

    while(1){

        scanKeys();
        int held = keysHeld();

        if(held & KEY_B){
            break;
		}

        secondDelay(0.05);

    }//end-while

    mmEffectEx(&doorStuck);
    secondDelay(0.5);
}