#include <nds.h>
#include <stdio.h>
#include <cstdlib>
#include <stack>
#include <ctime>
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"
#include "font.h"
#include "icons.h"
#include "titleScreen.h"
#include "levelOne.h"
#include "levelTwo.h"
#include "levelThree.h"
#include "levelFive.h"
#include "levelFour.h"
#include "instructions.h"
#include "death.h"
#include "constants.h"
#define QUOTECOUNT 9
#define HINTCOUNT 8

int goldAcquired=0;
int deathType=typeNone;
int totalScore=0;
int highscoreEasy=2000;
int highscoreNormal=3000;
int highscoreHard=4000;
int mode=NORMAL;
int tryEasy=250;
int tryNormal=500;
int tryHard=1000;

char *QUOTES[QUOTECOUNT]={
                            "Alright. Who wants some?",
                            "I'm a running buffet!",
                            "Zombies hate fast food.",
                            "Welcome to the party pal!",
                            "Choke on 'em!", "Groovy.",
                            "Ooh, he's got an arm off!",
                            "Time to nut up or shut up.",
                            "You've got red on you"
                            };
char *HINTS[HINTCOUNT]={
                        "Zombies hunt you down with    \n great speed when they sense   \n you.                          \n\n Keep your distance.",
                        "Raising the Game Difficulty   \n increases the Zombies         \n agressiveness and lowers your \n health. ",
                        "Stabbing the Zombies slows    \n them down.                    \n A tactical retreat is an      \n effective technique.",
                        "Sometimes it's better to leave\n the Gold.                     \n Rich and Dead is worthless.",
                        "Pick up Gold for points.",
                        "Avoid losing Hearts for bonus \n points.",
                        "Another try costs Points.     \n Make sure you can afford it.",
                        "When you hear a Zombie scream \n it's time to run.           \n It's about to chase you down."
                        };


int quotesUsed[4];
int hintsUsed[5];

//defines the size of the maze grid
const int gridJ=31; const int gridI=23;
bool victBool=false, deathBool=false;
int frameCounter=time(NULL);

using namespace std;
stack<int> location;

mm_sound_effect keyPickupSound, doorOpenSound, ladderSound, doorStuck, swordSound, zombieMoan, rockFall, hurt;

u16* mapMemory = (u16*)BG_MAP_RAM(0);
u8* tileMemory = (u8*)BG_TILE_RAM(1);

class tile {
    protected:
      int tileID;
    public:
      int solidity;
      int spriteRef;
      int reference;

      void make(int x, int y, int z, int sref){
        tileID=x, solidity=y, reference=z, spriteRef=sref;
      }
      void setSolidity(int solid){
           solidity=solid;
      }
      int getID(){
        return tileID;
      }
      int getSolidity(){
        return solidity;
      }
      int getRef(){
        return reference;
      }
      void resetRef(){
        reference=0;
      }
      void incRef(){
        reference++;
      }
      void setRef(int x){
        reference=x;
      }
      void setSprite(int x){
        spriteRef=x;
      }
      int getSprite(){
        return spriteRef;
      }
};
tile tileEmpty, tileWall, tileExplosion, tileRock;

class item: public tile{
    public:
        int I, J;
        bool status;
        int *anim;
        int counter;
        int sizeArray;

        void make(int x, int y, int ani[]){
            tileID=x, solidity=y, I=-1, J=-1, reference=0, anim=ani;
            status=false;
            counter=0;
            spriteRef=anim[0];

            sizeArray=0;
            while(anim[sizeArray]!=-1){
                sizeArray++;
            }
        }

        void incCounter(){
            counter++;
            counter=counter%sizeArray;
            spriteRef=anim[counter];
        }
        int getSpriteSize(){
            return sizeArray;
        }
        void defaultSprite(){
            spriteRef=anim[0];
            counter=0;
        }
        void setAnim(int ani[]){
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
        int * getAnim(){
            return anim;
        }
        int getI(){
            return I;
        }
        bool getStatus(){
            return status;
        }
        void setStatus(bool val){
            status=val;
        }
        int getJ(){
            return J;
        }
        void setI(int x){
            I=x;
        }
        void setJ(int x){
            J=x;
        }
};
item tileEnd, tileRDoor,tileRDoorBeta, tileRedButton, tileRedButtonBeta, tileGDoor, tileGDoorBeta, tileGreenButton,tileGreenButtonBeta, tileBlueButton,
                tileBlueDoor, tileBlueButtonBeta, tileBlueDoorBeta, tileOrangeDoor, tileOrangeDoorBeta, tileOrangeButton,
                tileOrangeButtonBeta, tileGold;

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

    void make(int x, int y, int a, int ani[]){
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
    void reset(){
        health=totalHealth;
        attackCharge=0;
        solidity=solidEnemy;
    }
    void resetHealth(){
        health=3;
    }
    bool getScream(){
        return scream;
    }
    void setScream(bool val){
        scream=val;
    }
    int getDelay(){
        return delay;
    }
    int getClock(){
        return clock;
    }
    void incClock(){
        if(I!=-1&&J!=-1){clock+=1;}
    }
    void setDelay(int del){
        delay = del;
    }
    void incCounter(){
            counter++;
            counter=counter%sizeArray;
            if(counter==0&&sizeArray>2){counter++;}
            spriteRef=anim[counter];

    }
    void defaultSprite(){
            spriteRef=anim[0];
        }
    void setAnim(int ani[]){
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
    int getI(){
        return I;
    }
    int getJ(){
        return J;
    }
    void setI(int x){
        I=x;
    }
    void setJ(int x){
        J=x;
    }
    int getHealth(){
        return health;
    }
    void setHealth(int h){
        health=h;
    }
    void takeHit(){
        if(health>0){health--;}
    }
    void move(tile grid[][gridJ],int dir, int &newI, int &newJ){
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
};
actor tileZombieA, tileZombieB,tileZombieC,tileZombieD,tileZombieE;

class player : public actor{
    public:
        int totalmoves;
        int *anim;
        int counter;

        int sizeArray;
        bool attacking;
        void make(int x, int y, int ani[]){
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
        void reset(){
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
        int getTotalHealth(){
            return totalHealth;
        }
        void incClock(){
            int breakLimit=15;
            int limit=20;

            if(attackCharge>breakLimit){
                attacking=false;
            }
            if(attackCharge<limit){attackCharge++;}

            clock+=1;
        }
        void attackUsed(){
            attackCharge=0;
            attacking=true;
        }
        bool getAttacking(){
            return attacking;
        }
        void cancelAttacking(){
            attacking=false;
        }
        int attackReady(){
            return attackCharge;
        }
        void incCounter(){
            counter++;
            counter=counter%sizeArray;
            if(counter==0&&sizeArray>2){counter++;}
            spriteRef=anim[counter];

        }
        int getSteps(){
            return totalmoves;
        }
        void takeHit(){
            if(health>0){health--;}
            mmEffectEx(&hurt);
        }
        void defaultSprite(){
            spriteRef=anim[0];
        }
        void setAnim(int ani[]){
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
        void resetSteps(){
            totalmoves=0;
        }
        void move(tile grid[][gridJ],int dir, int &newI, int &newJ){
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
};
player tileMazeMan;
//the tiles available in the game, all defined in main before runtime


void secondDelay(double I){
    int delay = I*60;
    for(int j=0;j<delay;j++){
            swiWaitForVBlank();
            frameCounter++;
    }

}

void printScreen(tile grid[][gridJ]){

    for(int i=0; i<24;i++){
        for(int j=0;j<32;j++){
            int t = (i*32)+j;

            if(i<23&&j<31){
                mapMemory[t]=grid[i][j].getSprite();
            }
            else{
                mapMemory[t]=spriteEmpty;
            }

        }
	}//end the stupid annoying loop.

    int currentHealth=tileMazeMan.getHealth();
    int position=766;

    for(int i=tileMazeMan.getTotalHealth();i>0;i--){
        if(((currentHealth)>=i)){mapMemory[position-i]=spriteHeartFull;}
        else{mapMemory[position-i]=spriteHeartEmpty;}
    }
}

void setFont(void){

    const int tile_base = 0;
	const int map_base = 20;

	videoSetModeSub(MODE_0_2D);
	vramSetBankC(VRAM_C_SUB_BG);

	PrintConsole *console = consoleInit(0,0, BgType_Text4bpp, BgSize_T_256x256, map_base, tile_base, false, false);

	ConsoleFont font;

	font.gfx = (u16*)fontTiles;
	font.pal = (u16*)fontPal;
	font.numChars = 95;
	font.numColors =  fontPalLen / 2;
	font.bpp = 4;
	font.asciiOffset = 32;
	font.convertSingleColor = false;

	consoleSetFont(console, &font);

}

void initTiles(){

    tileWall.make(tileWallID, solidSolid,1,spriteWall);
    tileRock.make(tileRockID, solidSolid,1,spriteRock);
    tileEnd.make(tileEndID,solidItem,animEnd);
    tileEmpty.make(tileEmptyID,solidEmpty,0,spriteEmpty);

    tileMazeMan.reset();

    tileRedButton.make(tileRedButtonID,solidItem,animRButton);
    tileRDoor.make(tileRDoorID,solidSolid,animDoorDown);
    tileRedButtonBeta.make(tileRedButtonBetaID,solidItem,animRButton);
    tileRDoorBeta.make(tileRDoorBetaID,solidSolid,animDoorDown);

    tileGreenButton.make(tileGreenButtonID,solidItem,animGButton);
    tileGDoor.make(tileGDoorID,solidSolid,animDoorGDown);
    tileGreenButtonBeta.make(tileGreenButtonBetaID,solidItem,animGButton);
    tileGDoorBeta.make(tileGDoorBetaID,solidSolid,animDoorGDown);

    tileBlueButton.make(tileBlueButtonID,solidItem,animBlueButton);
    tileBlueDoor.make(tileBlueDoorID,solidSolid,animDoorBlueDown);
    tileBlueButtonBeta.make(tileBlueButtonBetaID,solidItem,animBlueButton);
    tileBlueDoorBeta.make(tileBlueDoorBetaID,solidSolid,animDoorBlueDown);

    tileOrangeButton.make(tileOrangeButtonID,solidItem,animOrangeButton);
    tileOrangeDoor.make(tileOrangeDoorID,solidSolid,animDoorOrangeDown);
    tileOrangeButtonBeta.make(tileOrangeButtonBetaID,solidItem,animOrangeButton);
    tileOrangeDoorBeta.make(tileOrangeDoorBetaID,solidSolid,animDoorOrangeDown);

    tileGold.make(tileGoldID,solidItem,animGold);

    tileZombieA.make(tileZombieAID,solidEnemy,3,animZombieLeft);
    tileZombieB.make(tileZombieBID,solidEnemy,3,animZombieLeft);
    tileZombieC.make(tileZombieCID,solidEnemy,3,animZombieLeft);
    tileZombieD.make(tileZombieDID,solidEnemy,3,animZombieLeft);
    tileZombieE.make(tileZombieEID,solidEnemy,3,animZombieLeft);

}

void loadPalette(){

	BG_PALETTE[1] = RGB15(23,23,23);
	BG_PALETTE[2] = RGB15(21,4,4);
	BG_PALETTE[3] = RGB15(0,13,0);
	BG_PALETTE[4] = RGB15(0,0,28);
	BG_PALETTE[5] = RGB15(6,31,6);
	BG_PALETTE[6] = RGB15(16,8,8);
	BG_PALETTE[7] = RGB15(31,19,8);        //man flesh tone
	BG_PALETTE[8] = RGB15(0,0,0);           //black
	BG_PALETTE[9] = RGB15(31,31,31);        //white
	BG_PALETTE[10] = RGB15(6,6,6);
	BG_PALETTE[11] = RGB15(16,0,0);
	BG_PALETTE[12] = RGB15(31,0,0);         //perfect red
	BG_PALETTE[13] = RGB15(25,13,0);        //gold for the key
	BG_PALETTE[14] = RGB15(28,0,0);         //red for the button, darkest
	BG_PALETTE[15] = RGB15(25,0,0);         //red for the button, lighter
	BG_PALETTE[16] = RGB15(13,13,13);       //darkest grey
	BG_PALETTE[17] = RGB15(17,17,17);       //lighter grey
	BG_PALETTE[18] = RGB15(13,4,4);         //used for wall tiles
	BG_PALETTE[19] = RGB15(18,4,4);         //used for wall tiles
	BG_PALETTE[20] = RGB15(18,4,4);         //red button colour
    BG_PALETTE[21] = RGB15(3,3,3);         //darkest grey
    BG_PALETTE[22] = RGB15(6,6,6);         //darker grey
    BG_PALETTE[23] = RGB15(9,9,9);         //darkest grey
    BG_PALETTE[24] = RGB15(12,12,12);         //darkest grey
    BG_PALETTE[25] = RGB15(15,15,15);         //darkest grey
    BG_PALETTE[26] = RGB15(16,0,16);         //mazeman purple hair
    BG_PALETTE[27] = RGB15(0,5,0);         //dark green mm jacket
    BG_PALETTE[28] = RGB15(0,19,0);         //light mm green jacket

    BG_PALETTE[29] = RGB15(12,6,6);         //dark golem brown
    BG_PALETTE[30] = RGB15(19,9,9);         //mid golem brown
    BG_PALETTE[31] = RGB15(23,11,11);         //light golem brown
    BG_PALETTE[32] = RGB15(0,14,0);         //dark green mm jacket
    BG_PALETTE[33] = RGB15(7,15,7);         //mid zombie green
    BG_PALETTE[34] = RGB15(13,17,9);         //light yellow/green
    BG_PALETTE[35] = RGB15(16,0,3);         //blood red
    BG_PALETTE[36] = RGB15(16,16,16);
    BG_PALETTE[37] = RGB15(18,18,18);

    BG_PALETTE[38] = RGB15(21,4,4);         //used for wall tiles
    BG_PALETTE[39] = RGB15(15,4,4);         //used for wall tiles
    BG_PALETTE[40] = RGB15(20,20,20);         //used for wall tiles
    BG_PALETTE[41] = RGB15(22,22,22);         //used for wall tiles
    BG_PALETTE[42] = RGB15(22,4,4);

    BG_PALETTE[43] = RGB15(4,4,21);         //darker blue
    BG_PALETTE[44] = RGB15(9,9,31);         //lighter blue

    BG_PALETTE[45] = RGB15(23,9,0);         //darker ORANGE
    BG_PALETTE[46] = RGB15(31,12,0);         //lighter orange


}

void loadIconsToMemory(){

    swiCopy(emptyICO,tileMemory,32);
	swiCopy(redButtonUpICO,tileMemory+64,32);
	swiCopy(redButtonDownICO,tileMemory+128,32);
	swiCopy(greenButtonUpICO,tileMemory+192,32);
    swiCopy(doorDownICO,tileMemory+256,32);
    swiCopy(wallICO,tileMemory+320,32);
    swiCopy(endDownICO,tileMemory+384,32);
    swiCopy(manDownAICO,tileMemory+448,32);
    swiCopy(manUpAICO,tileMemory+512,32);
    swiCopy(manLeftICO,tileMemory+576,32);
    swiCopy(greenButtonMiddleICO,tileMemory+640,32);
    swiCopy(doorLeftICO,tileMemory+704,32);
    swiCopy(doorRightICO,tileMemory+768,32);

    swiCopy(wallUpICO,tileMemory+832,32);
    swiCopy(wallDownICO,tileMemory+896,32);
    swiCopy(wallLeftICO,tileMemory+960,32);
    swiCopy(wallRightICO,tileMemory+1024,32);
    swiCopy(wallUpDownICO,tileMemory+1088,32);
    swiCopy(wallUpLeftICO,tileMemory+1152,32);
    swiCopy(wallUpRightICO,tileMemory+1216,32);
    swiCopy(wallDownLeftICO,tileMemory+1280,32);
    swiCopy(wallDownRightICO,tileMemory+1344,32);
    swiCopy(wallLeftRightICO,tileMemory+1408,32);
    swiCopy(wallUpDownLeftRightICO,tileMemory+1472,32);
    swiCopy(wallUpLeftDownICO,tileMemory+1536,32);
    swiCopy(wallUpLeftRightICO,tileMemory+1600,32);
    swiCopy(wallUpRightDownICO,tileMemory+1664,32);
    swiCopy(wallDownLeftRightICO,tileMemory+1728,32);
    swiCopy(wallIslandICO,tileMemory+1792,32);

    swiCopy(redButtonMiddleICO,tileMemory+1856,32);
    swiCopy(doorLeftBICO,tileMemory+1920,32);
    swiCopy(doorLeftCICO,tileMemory+1984,32);
    swiCopy(manDownBICO,tileMemory+2048,32);
    swiCopy(manUpBICO,tileMemory+2112,32);
    swiCopy(manDownICO,tileMemory+2176,32);
    swiCopy(manUpICO,tileMemory+2240,32);
    swiCopy(manLeftAICO,tileMemory+2304,32);
    swiCopy(manLeftBICO,tileMemory+2368,32);
    swiCopy(greenButtonDownICO,tileMemory+2432,32);
    swiCopy(rockICO,tileMemory+2496,32);
    swiCopy(doorDownBICO,tileMemory+2560,32);
    swiCopy(doorDownCICO,tileMemory+2624,32);
    swiCopy(doorDownDICO,tileMemory+2688,32);
    swiCopy(doorLeftDICO,tileMemory+2752,32);
    swiCopy(doorRightBICO,tileMemory+2816,32);
    swiCopy(doorRightCICO,tileMemory+2880,32);
    swiCopy(doorRightDICO,tileMemory+2944,32);
    swiCopy(endDownBICO,tileMemory+3008,32);
    swiCopy(endDownCICO,tileMemory+3072,32);
    swiCopy(endDownDICO,tileMemory+3136,32);
    swiCopy(endDownEICO,tileMemory+3200,32);

    swiCopy(doorGDownICO,tileMemory+3840,32);
    swiCopy(doorGDownBICO,tileMemory+3904,32);
    swiCopy(doorGDownCICO,tileMemory+3968,32);
    swiCopy(doorGDownDICO,tileMemory+4032,32);
    swiCopy(doorGLeftICO,tileMemory+4096,32);
    swiCopy(doorGLeftBICO,tileMemory+4160,32);
    swiCopy(doorGLeftCICO,tileMemory+4224,32);
    swiCopy(doorGLeftDICO,tileMemory+4288,32);
    swiCopy(doorGRightICO,tileMemory+4352,32);
    swiCopy(doorGRightBICO,tileMemory+4416,32);
    swiCopy(doorGRightCICO,tileMemory+4480,32);
    swiCopy(doorGRightDICO,tileMemory+4544,32);

    swiCopy(blueButtonUpICO,tileMemory+4608,32);
    swiCopy(blueButtonMiddleICO,tileMemory+4672,32);
    swiCopy(blueButtonDownICO,tileMemory+4736,32);

    swiCopy(manLeftAttackICO,tileMemory+4864,32);
    swiCopy(manRightAttackICO,tileMemory+4928,32);
    swiCopy(manUpAttackICO,tileMemory+4992,32);
    swiCopy(manDownAttackICO,tileMemory+5056,32);

    swiCopy(zombieUpICO,tileMemory+5120,32);
    swiCopy(zombieUpBICO,tileMemory+5184,32);
    swiCopy(zombieUpCICO,tileMemory+5248,32);
    swiCopy(zombieLeftICO,tileMemory+5312,32);
    swiCopy(zombieLeftBICO,tileMemory+5376,32);
    swiCopy(zombieLeftCICO,tileMemory+5440,32);
    swiCopy(zombieRightICO,tileMemory+5504,32);
    swiCopy(zombieRightBICO,tileMemory+5568,32);
    swiCopy(zombieRightCICO,tileMemory+5632,32);
    swiCopy(zombieBLeftICO,tileMemory+5696,32);
    swiCopy(zombieBLeftBICO,tileMemory+5760,32);
    swiCopy(zombieBLeftCICO,tileMemory+5824,32);
    swiCopy(zombieBUpICO,tileMemory+5888,32);
    swiCopy(zombieBUpBICO,tileMemory+5952,32);
    swiCopy(zombieBUpCICO,tileMemory+6016,32);
    swiCopy(zombieBRightICO,tileMemory+6080,32);
    swiCopy(zombieBRightBICO,tileMemory+6144,32);
    swiCopy(zombieBRightCICO,tileMemory+6208,32);
    swiCopy(zombieCUpICO,tileMemory+6272,32);
    swiCopy(zombieCUpBICO,tileMemory+6336,32);
    swiCopy(zombieCUpCICO,tileMemory+6400,32);
    swiCopy(zombieCLeftICO,tileMemory+6464,32);
    swiCopy(zombieCLeftBICO,tileMemory+6528,32);
    swiCopy(zombieCLeftCICO,tileMemory+6592,32);
    swiCopy(zombieCRightICO,tileMemory+6656,32);
    swiCopy(zombieCRightBICO,tileMemory+6720,32);
    swiCopy(zombieCRightCICO,tileMemory+6784,32);
    swiCopy(zombieDeadICO,tileMemory+6848,32);
    swiCopy(heartFullICO,tileMemory+6912,32);
    swiCopy(heartEmptyICO,tileMemory+6976,32);
    swiCopy(doorDownBlueICO,tileMemory+7040,32);
    swiCopy(doorDownBlueBICO,tileMemory+7104,32);
    swiCopy(doorDownBlueCICO,tileMemory+7168,32);
    swiCopy(doorDownBlueDICO,tileMemory+7232,32);
    swiCopy(doorLeftBlueICO,tileMemory+7296,32);
    swiCopy(doorLeftBlueBICO,tileMemory+7360,32);
    swiCopy(doorLeftBlueCICO,tileMemory+7424,32);
    swiCopy(doorLeftBlueDICO,tileMemory+7488,32);
    swiCopy(doorRightBlueICO,tileMemory+7552,32);
    swiCopy(doorRightBlueBICO,tileMemory+7616,32);
    swiCopy(doorRightBlueCICO,tileMemory+7680,32);
    swiCopy(doorRightBlueDICO,tileMemory+7744,32);
    swiCopy(doorOrangeDownICO,tileMemory+7808,32);
    swiCopy(doorOrangeDownBICO,tileMemory+7872,32);
    swiCopy(doorOrangeDownCICO,tileMemory+7936,32);
    swiCopy(doorOrangeDownDICO,tileMemory+8000,32);
    swiCopy(doorOrangeLeftICO,tileMemory+8064,32);
    swiCopy(doorOrangeLeftBICO,tileMemory+8128,32);
    swiCopy(doorOrangeLeftCICO,tileMemory+8192,32);
    swiCopy(doorOrangeLeftDICO,tileMemory+8256,32);
    swiCopy(doorOrangeRightICO,tileMemory+8320,32);
    swiCopy(doorOrangeRightBICO,tileMemory+8384,32);
    swiCopy(doorOrangeRightCICO,tileMemory+8448,32);
    swiCopy(doorOrangeRightDICO,tileMemory+8512,32);
    swiCopy(orangeButtonUpICO,tileMemory+8576,32);
    swiCopy(orangeButtonMiddleICO,tileMemory+8640,32);
    swiCopy(orangeButtonDownICO,tileMemory+8704,32);
    swiCopy(goldICO,tileMemory+9024,32);


}

void initSounds(){

    mmLoadEffect( SFX_KEY );
    mmLoadEffect( SFX_DOOROPEN );
    mmLoadEffect( SFX_DOORSTUCK);
    mmLoadEffect( SFX_LADDER);
    mmLoadEffect( SFX_SWORD);
    mmLoadEffect( SFX_ZOMBIE);
    mmLoadEffect( SFX_HURT);
    mmLoadEffect( SFX_ROCKFALL);

    hurt = {
		{ SFX_HURT } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		180,	// volume
		128,	// panning
	};

    rockFall = {
		{ SFX_ROCKFALL } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		60,	// volume
		128,	// panning
	};

    keyPickupSound = {
		{ SFX_KEY } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};
    doorOpenSound = {
		{ SFX_DOOROPEN } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};
    ladderSound = {
		{ SFX_LADDER } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};
    swordSound = {
		{ SFX_SWORD } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};
    doorStuck = {
		{ SFX_DOORSTUCK } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};
    zombieMoan = {
    		{ SFX_ZOMBIE } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};

}

void showPicture(int pic){
    videoSetMode(MODE_5_2D);

    int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

    if(pic==picMenu){
        dmaCopy(titleScreenBitmap, bgGetGfxPtr(bg3), 256*256);
        dmaCopy(titleScreenPal, BG_PALETTE, 256*2);
    }
    else if(pic==picOne){
        dmaCopy(levelOneBitmap, bgGetGfxPtr(bg3), 256*256);
        dmaCopy(levelOnePal, BG_PALETTE, 256*2);
    }
   else if(pic==picTwo){
        dmaCopy(levelTwoBitmap, bgGetGfxPtr(bg3), 256*256);
        dmaCopy(levelTwoPal, BG_PALETTE, 256*2);
    }
    else if(pic==picThree){
        dmaCopy(levelThreeBitmap, bgGetGfxPtr(bg3), 256*256);
        dmaCopy(levelThreePal, BG_PALETTE, 256*2);
    }
    else if(pic==picFour){
        dmaCopy(levelFourBitmap, bgGetGfxPtr(bg3), 256*256);
        dmaCopy(levelFourPal, BG_PALETTE, 256*2);
    }
    else if(pic==picFive){
        dmaCopy(levelFiveBitmap, bgGetGfxPtr(bg3), 256*256);
        dmaCopy(levelFivePal, BG_PALETTE, 256*2);
    }
    else if(pic==picInst){
        dmaCopy(instructionsBitmap, bgGetGfxPtr(bg3), 256*256);
        dmaCopy(instructionsPal, BG_PALETTE, 256*2);
    }
    else if(pic==picDeath){
        dmaCopy(deathBitmap, bgGetGfxPtr(bg3), 256*256);
        dmaCopy(deathPal, BG_PALETTE, 256*2);
    }
}

void setScreenForPlay(){
    videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_BG1_ACTIVE);
	REG_BG0CNT = BG_32x32 | BG_COLOR_256 | BG_MAP_BASE(0) | BG_TILE_BASE(1);
}

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

//###################################################################
//####################       MAZE CREATION   ########################
//###################################################################*/
void intactCheck(tile grid[][gridJ], int I, int J,int &checkUp, int &checkDown, int &checkLeft, int &checkRight){

    checkRight=0;checkLeft=0;checkUp=0;checkDown=0;
    //get the direction to check
    if(grid[I][J+2].getRef()==0&&(I+2<gridJ)){
      if(grid[I][J+3].getID()==tileWallID&&grid[I+1][J+1].getID()==tileWallID&&
         grid[I-1][J+1].getID()==tileWallID&&grid[I+1][J+2].getID()==tileWallID){
            checkRight=1;
         }
    }

    if(grid[I][J-2].getRef()==0&&(J-2>0)){
      if(grid[I][J-3].getID()==tileWallID&&grid[I][J-1].getID()==tileWallID&&
         grid[I-1][J-2].getID()==tileWallID&&grid[I+1][J-2].getID()==tileWallID){
            checkLeft=1;
         }
    }

    if(grid[I+2][J].getRef()==0&&(I+2<gridI)){
      if(grid[I+3][J].getID()==tileWallID&&grid[I+1][J].getID()==tileWallID&&
         grid[I+2][J+1].getID()==tileWallID&&grid[I+2][J-1].getID()==tileWallID){
            checkDown=1;
         }
    }

    if(grid[I-2][J].getRef()==0&&(I-2>0)){
      if(grid[I-3][J].getID()==tileWallID&&grid[I-1][J].getID()==tileWallID&&
         grid[I-2][J-1].getID()==tileWallID&&grid[I-2][J+1].getID()==tileWallID){
            checkUp=1;
        }
    }
}//end-intact check;

void wallBreak(tile grid[][gridJ], int &mazeI, int &mazeJ, int dir, int &visitedcells){

	//save the current location
    location.push(mazeJ);
    location.push(mazeI);

    //get the direction to break down and move to
    switch(dir){
        case UP:

			grid[mazeI-1][mazeJ]=tileEmpty;
            mazeI-=2;
            break;
        case DOWN:

			grid[mazeI+1][mazeJ]=tileEmpty;
            mazeI+=2;
            break;
        case LEFT:

			grid[mazeI][mazeJ-1]=tileEmpty;
            mazeJ-=2;
            break;
        case RIGHT:

			grid[mazeI][mazeJ+1]=tileEmpty;
            mazeJ+=2;
            break;
    }


    grid[mazeI][mazeJ].setRef(1);
    //increment the total visited
    visitedcells++;
}

void findAdjacentEmptyTiles(tile grid[][gridJ], int I, int J, int &up, int &down, int &left, int &right){
    up=down=left=right=0;
        if((grid[I][J+1].getSolidity()!=solidSolid&&grid[I][J+1].getSolidity()!=solidItem)&&grid[I][J+1].getID()!=tileRockID){
            right=1;
        }
        if((grid[I][J-1].getSolidity()!=solidSolid&&grid[I][J-1].getSolidity()!=solidItem)&&grid[I][J-1].getID()!=tileRockID){
            left=1;
        }
        if((grid[I-1][J].getSolidity()!=solidSolid&&grid[I-1][J].getSolidity()!=solidItem)&&grid[I-1][J].getID()!=tileRockID){
            up=1;
        }
        if((grid[I+1][J].getSolidity()!=solidSolid&&grid[I+1][J].getSolidity()!=solidItem)&&grid[I+1][J].getID()!=tileRockID){
            down=1;
        }

        if(J==0){
            left=1;
        }
        if(I==0){
            up=1;
        }
        if(J==gridJ-1){
            right=1;
        }
        if(I==gridI-1){
            down=1;
        }
}//end

void setWallSprites(tile grid[][gridJ]){

    int checkUp,checkDown,checkLeft,checkRight;
    //now, we're going to go trough all the wall tiles and set them up with the correct sprites.
    //So that they look all sexy and stuff.
    for(int i=0;i<gridI;i++){
        for(int j=0;j<gridJ;j++){
            if(grid[i][j].getID()==tileWallID){
                findAdjacentEmptyTiles(grid,i,j,checkUp,checkDown,checkLeft,checkRight);
                if(checkUp==0&&checkDown==1&&checkLeft==1&&checkRight==1){
                    grid[i][j].setSprite(spriteWallU);
                }
                else if(checkUp==1&&checkDown==0&&checkLeft==1&&checkRight==1){
                    grid[i][j].setSprite(spriteWallD);
                }
                else if(checkUp==1&&checkDown==1&&checkLeft==0&&checkRight==1){
                    grid[i][j].setSprite(spriteWallL);
                }
                else if(checkUp==1&&checkDown==1&&checkLeft==1&&checkRight==0){
                    grid[i][j].setSprite(spriteWallR);
                }
                else if(checkUp==0&&checkDown==0&&checkLeft==1&&checkRight==1){
                    grid[i][j].setSprite(spriteWallUD);
                }
                else if(checkUp==0&&checkDown==1&&checkLeft==0&&checkRight==1){
                    grid[i][j].setSprite(spriteWallUL);
                }
                else if(checkUp==0&&checkDown==1&&checkLeft==1&&checkRight==0){
                    grid[i][j].setSprite(spriteWallUR);
                }
                else if(checkUp==1&&checkDown==0&&checkLeft==0&&checkRight==1){
                    grid[i][j].setSprite(spriteWallDL);
                }
                else if(checkUp==1&&checkDown==0&&checkLeft==1&&checkRight==0){
                    grid[i][j].setSprite(spriteWallDR);
                }
                else if(checkUp==1&&checkDown==1&&checkLeft==0&&checkRight==0){
                    grid[i][j].setSprite(spriteWallLR);
                }
                else if(checkUp==0&&checkDown==0&&checkLeft==0&&checkRight==0){
                    grid[i][j].setSprite(spriteWallUDLR);
                }
                else if(checkUp==1&&checkDown==0&&checkLeft==0&&checkRight==0){
                    grid[i][j].setSprite(spriteWallDLR);
                }
                else if(checkUp==0&&checkDown==0&&checkLeft==0&&checkRight==1){
                    grid[i][j].setSprite(spriteWallULD);
                }
                else if(checkUp==0&&checkDown==1&&checkLeft==0&&checkRight==0){
                    grid[i][j].setSprite(spriteWallULR);
                }
                else if(checkUp==0&&checkDown==0&&checkLeft==1&&checkRight==0){
                    grid[i][j].setSprite(spriteWallURD);
                }
                else if(checkUp==1&&checkDown==1&&checkLeft==1&&checkRight==1){
                    grid[i][j].setSprite(spriteWallSolo);
                }
            }
        }
    }//end
}

void makeMaze(tile grid[][gridJ], bool braid){

    int mazeI,mazeJ;
    int visitedcells=0;
    //makes grid
    for(int i=0;i<gridI;i++){
        for(int j=0;j<gridJ;j++){
            if(i%2==0||j%2==0){
                grid[i][j]=tileWall;
            }
            else{
                grid[i][j]=tileEmpty;
            }

        }
    }//end-grid-creation

    //count the total cells and prepare maze start
    int totalcells = ((gridI-1)*(gridJ-1))/4;
    //build the maze from position 1,1
    mazeI = mazeJ = 1;
    grid[mazeI][mazeJ].reference=1;
    visitedcells=1;
    srand(frameCounter);

    int checkUp,checkDown,checkLeft,checkRight;
    int randDir;

    //until every cell in the maze is accessible.
    while(visitedcells<totalcells){

        //checks each of the directions to see which neighbouring cells have all intact walls
        intactCheck(grid, mazeI, mazeJ,checkUp,checkDown,checkLeft,checkRight);

        if(checkUp==0&&checkDown==0&&checkLeft==0&&checkRight==0){
            mazeI = location.top();
            location.pop();
            mazeJ = location.top();
            location.pop();
        }
        else{
            while(1){
                randDir = rand() % 4;
                if(randDir==0&&checkUp==1){
                    break;
                }
                else if(randDir==1&&checkDown==1){
                    break;
                }
                else if(randDir==2&&checkLeft==1){
                    break;
                }
                else if(randDir==3&&checkRight==1){
                    break;
                }
            }//found a decent direction to break
            wallBreak(grid,mazeI,mazeJ,randDir,visitedcells);
        }
    }//end maze generation while*/

    if(braid==true){
        //randomly tear down a few walls, played hell with the path finding
        int randI=0;
        int randJ=0;
        int up, down, left, right;
        for(int i=0;i<15;i++){
            while(1){
                while(1){
                    randI = rand()%gridI;
                    randJ = rand()%gridJ;
                    if(grid[randI][randJ].getSolidity()==solidSolid&&randJ>2&&randJ<gridJ-1&&randI>2&&randI<gridI-1){
                        break;
                    }
                }

                findAdjacentEmptyTiles(grid,randI,randJ,up,down,left,right);
                if((up==1&&down==1&&left==0&&right==0)||(up==0&&down==0&&left==1&&right==1)){
                    grid[randI][randJ]=tileEmpty;
                    break;
                }
            }
        }
    }

    setWallSprites(grid);

}//end mazeGen funct

//###################################################################
//###################       MAZE POPULATION   #######################
//###################################################################

int findRoute(tile grid[][gridJ],int currentI, int currentJ, int desiredI, int desiredJ,
    int &stepsRequired, int &direction){

    int returnval=1;
    //just to quit some errors while i was debuggin
    if(currentI==desiredI&&currentJ==desiredJ){return returnval;}

    //go through and set all references to 0
    for(int i=0;i<gridI;i++){
        for(int j=0;j<gridJ;j++){
                grid[i][j].setRef(0);
        }
    }

    //set the starting reference to point 1
    grid[currentI][currentJ].setRef(1);

    //the switch to get out of the infinite loop
    bool loopswitch = false;

    //these two stacks will be used to keep track of the open ends.
    stack<int> endStack;
    stack<int> nextEndStack;

    //put the starting tile into the endStack
    endStack.push(currentI);
    endStack.push(currentJ);

    //time to start hunting through the maze!
    while (loopswitch==false){

        int tempI=0,tempJ=0;
        if(endStack.empty()){
            returnval=-1;
            loopswitch=true;
            break;
        }

        while(!endStack.empty()){
            tempJ=endStack.top();
            endStack.pop();
            tempI=endStack.top();
            endStack.pop();

            //if this piece is the desired slot, then by gosh we've found our way!
            if(tempI==desiredI&&tempJ==desiredJ){

                stepsRequired=grid[tempI][tempJ].getRef()-1;

                //this bit traces back until we find the bit labelled #2, this will be the
                //direction we have to take in order to get closer to the sweet spot
                while(grid[tempI][tempJ].getRef()!=2){
                    int currentRefMinusOne = grid[tempI][tempJ].getRef()-1;
                    //look right
                    if(grid[tempI][tempJ+1].getRef()==currentRefMinusOne){
                        tempJ++;
                    }
                    //look left
                    else if(grid[tempI][tempJ-1].getRef()==currentRefMinusOne){
                        tempJ--;
                    }
                    //look up
                    else if(grid[tempI-1][tempJ].getRef()==currentRefMinusOne){
                        tempI--;
                    }
                    //look down
                    else if(grid[tempI+1][tempJ].getRef()==currentRefMinusOne){
                        tempI++;
                    }
                }//end tracing back while loop

                //decides which direction we have to turn!
                if(currentI==tempI&&currentJ==tempJ+1){
                    direction=LEFT;
                }
                else if(currentI==tempI&&currentJ==tempJ-1){
                    direction=RIGHT;
                }
                else if(currentI==tempI-1&&currentJ==tempJ){
                    direction=DOWN;
                }
                else if(currentI==tempI+1&&currentJ==tempJ){
                    direction=UP;
                }
                //
                //mmEffectEx(&golemScream);
                loopswitch=true;
                break;

            }//end found desired tile

            //scope around the current tile, see if there are any tiles we can move to
            //add a +1 to their reference.
            int currentTilePlusOne = grid[tempI][tempJ].getRef()+1;

            //look right
            if(grid[tempI][tempJ+1].getSolidity()!=solidSolid&&grid[tempI][tempJ+1].getSolidity()!=solidItem&&grid[tempI][tempJ+1].getSolidity()!=solidEnemy&&grid[tempI][tempJ+1].getRef()==0){
                grid[tempI][tempJ+1].setRef(currentTilePlusOne);
                //add this as an open end
                nextEndStack.push(tempI);
                nextEndStack.push(tempJ+1);
            }
            //look left
            if(grid[tempI][tempJ-1].getSolidity()!=solidSolid&&grid[tempI][tempJ-1].getSolidity()!=solidItem&&grid[tempI][tempJ-1].getSolidity()!=solidEnemy&&grid[tempI][tempJ-1].getRef()==0){
                grid[tempI][tempJ-1].setRef(currentTilePlusOne);
                nextEndStack.push(tempI);
                nextEndStack.push(tempJ-1);
            }
            //look up
            if(grid[tempI-1][tempJ].getSolidity()!=solidSolid&&grid[tempI-1][tempJ].getSolidity()!=solidItem&&grid[tempI-1][tempJ].getSolidity()!=solidEnemy&&grid[tempI-1][tempJ].getRef()==0){
                grid[tempI-1][tempJ].setRef(currentTilePlusOne);
                nextEndStack.push(tempI-1);
                nextEndStack.push(tempJ);
            }
            //look down
            if(grid[tempI+1][tempJ].getSolidity()!=solidSolid&&grid[tempI+1][tempJ].getSolidity()!=solidItem&&grid[tempI+1][tempJ].getSolidity()!=solidEnemy&&grid[tempI+1][tempJ].getRef()==0){
                grid[tempI+1][tempJ].setRef(currentTilePlusOne);
                nextEndStack.push(tempI+1);
                nextEndStack.push(tempJ);
            }

        }//end reference tile search

        //swap the stacks around, empty the temp stack.
        endStack = nextEndStack;
        while(!nextEndStack.empty()){
            nextEndStack.pop();
        }
    }//end the global while, the one controlled by the switch. Cant be quit without finding a way

    return returnval;

}
//end path finder Dijkstra’s Algorithm!

//returns the location of the tile which would allow you to make a choice, a new direction
void walkToNextJunct(tile grid[][gridJ], int I, int J, int &junctI, int &junctJ, int &direction, int &steps){

    steps=0;
    int up,down,left,right;
    int returnI=I;
    int returnJ=J;
    bool firstMove=true;
    int takenDirection=direction;

    findAdjacentEmptyTiles(grid,returnI,returnJ,up,down,left,right);
    int dirTotal=up+down+left+right;

    if(direction==UP&&grid[I-1][J].getID()!=tileEmptyID&&dirTotal==1){
        direction=DOWN;
        steps=0;
        return;
    }
    else if(direction==DOWN&&grid[I+1][J].getID()!=tileEmptyID&&dirTotal==1){
       direction=UP;
       steps=0;
        return;
    }
    else if(direction==LEFT&&grid[I][J-1].getID()!=tileEmptyID&&dirTotal==1){
        direction=RIGHT;
        steps=0;
        return;
    }
    else if(direction==RIGHT&&grid[I][J+1].getID()!=tileEmptyID&&dirTotal==1){
        direction=LEFT;
        steps=0;
        return;
    }

    while(1){

        findAdjacentEmptyTiles(grid,returnI,returnJ,up,down,left,right);
        int dirTotal=up+down+left+right;

        if(dirTotal>2){
            break;
        }

        if(takenDirection==UP&&grid[returnI-1][returnJ].getID()!=tileEmptyID&&dirTotal==1){
            takenDirection=DOWN;
            break;
        }
        else if(takenDirection==DOWN&&grid[returnI+1][returnJ].getID()!=tileEmptyID&&dirTotal==1){
            takenDirection=UP;
            break;
        }
        else if(takenDirection==LEFT&&grid[returnI][returnJ-1].getID()!=tileEmptyID&&dirTotal==1){
            takenDirection=RIGHT;
            break;
        }
        else if(takenDirection==RIGHT&&grid[returnI][returnJ+1].getID()!=tileEmptyID&&dirTotal==1){
            takenDirection=LEFT;
            break;
        }


        if(takenDirection==DOWN){
            if(grid[returnI+1][returnJ].getSolidity()==solidSolid||grid[returnI+1][returnJ].getSolidity()==solidItem){

                if(left==1&&right==0){
                    if(firstMove==true){
                        direction=LEFT;
                        firstMove=false;
                    }
                    takenDirection=LEFT;
                }
                else if(left==0&&right==1){
                    if(firstMove==true){
                        direction=RIGHT;
                        firstMove=false;
                    }
                    takenDirection=RIGHT;
                }
                                else{
                    direction=STOP;return;
                }

            }
            else{
                if(firstMove==true){
                    direction=DOWN;
                    firstMove=false;
                }
                returnI++;
                steps++;
            }
        }//end down
        else if(takenDirection==RIGHT){
            if(grid[returnI][returnJ+1].getSolidity()==solidSolid||grid[returnI][returnJ+1].getSolidity()==solidItem){

                if(down==1&&up==0){
                    if(firstMove==true){
                        direction=DOWN;
                        firstMove=false;
                    }
                    takenDirection=DOWN;
                }
                else if(down==0&&up==1){
                    if(firstMove==true){
                        direction=UP;
                        firstMove=false;
                    }
                    takenDirection=UP;
                }
                else{
                    direction=STOP;return;
                }

            }
            else{
                if(firstMove==true){
                    direction=RIGHT;
                    firstMove=false;
                }
                returnJ++;
                steps++;
            }
        }// end right
        else if(takenDirection==LEFT){
            if(grid[returnI][returnJ-1].getSolidity()==solidSolid||grid[returnI][returnJ-1].getSolidity()==solidItem){

                if(down==1&&up==0){
                    if(firstMove==true){
                        direction=DOWN;
                        firstMove=false;
                    }
                    takenDirection=DOWN;
                }
                else if(down==0&&up==1){
                    if(firstMove==true){
                        direction=UP;
                        firstMove=false;
                    }
                    takenDirection=UP;
                }
                                else{
                    direction=STOP;return;
                }

            }
            else{
                if(firstMove==true){
                    direction=LEFT;
                    firstMove=false;
                }
                returnJ--;
                steps++;
            }
        }// end LEFT
        else if(takenDirection==UP){
            if(grid[returnI-1][returnJ].getSolidity()==solidSolid||grid[returnI-1][returnJ].getSolidity()==solidItem){

                if(left==1&&right==0){
                    if(firstMove==true){
                        direction=LEFT;
                        firstMove=false;
                    }
                    takenDirection=LEFT;
                }
                else if(left==0&&right==1){
                    if(firstMove==RIGHT){
                        direction=LEFT;
                        firstMove=false;
                    }
                    takenDirection=RIGHT;
                }
                                else{
                    direction=STOP;return;
                }

            }
            else{
                if(firstMove==true){
                    direction=UP;
                    firstMove=false;
                }
                returnI--;
                steps++;
            }
        }//end up

    }//end loop

    junctI=returnI;
    junctJ=returnJ;
}//end


//###################################################################
//###################      OTHER / MAIN     #########################
//###################################################################

void animateItem(tile grid[][gridJ],item &T, double delay, mm_sound_effect sfx ){

    int frames = T.getSpriteSize();
    for(int i=0; i<frames-1; i++){

        T.incCounter();
        grid[T.getI()][T.getJ()]=T;
        printScreen(grid);
        mmEffectEx(&sfx);
        secondDelay(delay);
    }
}

void animateItem(tile grid[][gridJ],item *&T, double delay, mm_sound_effect sfx ){

    int frames = T->getSpriteSize();
    for(int i=0; i<frames-1; i++){

        T->incCounter();
        grid[T->getI()][T->getJ()]=*T;
        printScreen(grid);
        mmEffectEx(&sfx);
        secondDelay(delay);
    }
}

void animateDual(tile grid[][gridJ],item *&T1, item *&T2, double delay, mm_sound_effect sfx ){

    int frames = T1->getSpriteSize();

    if(frames!=T2->getSpriteSize()){return;}

    for(int i=0; i<frames-1; i++){

        T1->incCounter();
        T2->incCounter();
        grid[T1->getI()][T1->getJ()]=*T1;
        grid[T2->getI()][T2->getJ()]=*T2;
        printScreen(grid);
        mmEffectEx(&sfx);
        secondDelay(delay);
    }
}

void animateItem(tile grid[][gridJ],item *&T, double delay ){

    int frames = T->getSpriteSize();
    for(int i=0; i<frames-1; i++){

        T->incCounter();
        grid[T->getI()][T->getJ()]=*T;
        printScreen(grid);
        secondDelay(delay);
    }
}

void animateDual(tile grid[][gridJ],item *&T1,item *&T2, double delay ){

    int frames = T1->getSpriteSize();

    if(frames!=T2->getSpriteSize()){return;}

    for(int i=0; i<frames-1; i++){

        T1->incCounter();
        T2->incCounter();
        grid[T1->getI()][T1->getJ()]=*T1;
        grid[T2->getI()][T2->getJ()]=*T2;
        printScreen(grid);
        secondDelay(delay);
    }
}

void animateItem(tile grid[][gridJ],item &T, double delay ){

    int frames = T.getSpriteSize();
    for(int i=0; i<frames-1; i++){

        T.incCounter();
        grid[T.getI()][T.getJ()]=T;
        printScreen(grid);
        secondDelay(delay);
    }
}

//moves the enemy tiles, relevant collision detection
void actorMove(tile grid[][gridJ],actor &T, int direction, int manDir, mm_sound_effect sfx){

    int tempID=0; int tempSolidity=0;
    int *animUp=animEnd;
    int *animDown=animEnd;
    int *animLeft=animEnd;
    int *animRight=animEnd;

    T.incCounter();
    if(T.getID()==tileZombieAID||T.getID()==tileZombieBID||T.getID()==tileZombieCID||T.getID()==tileZombieDID||
            T.getID()==tileZombieEID){
        if(T.getHealth()==3){
            animUp=animZombieUp;
            animDown=animZombieDown;
            animLeft=animZombieLeft;
            animRight=animZombieRight;
        }
        else if (T.getHealth()==2){
            animUp=animZombieBUp;
            animDown=animZombieBDown;
            animLeft=animZombieBLeft;
            animRight=animZombieBRight;
        }
        else if (T.getHealth()==1){
            animUp=animZombieCUp;
            animDown=animZombieCDown;
            animLeft=animZombieCLeft;
            animRight=animZombieCRight;
        }
    }

    if(direction==UP){
        T.setAnim(animUp);
        tempID=grid[T.getI()-1][T.getJ()].getID();
        tempSolidity=grid[T.getI()-1][T.getJ()].getSolidity();
    }
    else if(direction==DOWN){
        T.setAnim(animDown);
        tempID=grid[T.getI()+1][T.getJ()].getID();
        tempSolidity=grid[T.getI()+1][T.getJ()].getSolidity();
    }
    else if(direction==LEFT){
        T.setAnim(animLeft);
        tempID=grid[T.getI()][T.getJ()-1].getID();
        tempSolidity=grid[T.getI()][T.getJ()-1].getSolidity();
    }
    else if(direction==RIGHT){
        T.setAnim(animRight);
        tempID=grid[T.getI()][T.getJ()+1].getID();
        tempSolidity=grid[T.getI()][T.getJ()+1].getSolidity();
    }
    //enforce the updated animation
    grid[T.getI()][T.getJ()]=T;

    //check for obstacles/objects etc in the direction
    int dir=direction;
    int cI=T.getI();
    int cJ=T.getJ();
    //go there if its empty
    if(tempSolidity==solidEmpty){
        T.move(grid,dir,cI,cJ);
        grid[T.getI()][T.getJ()]=T;
    }
    else if(tempSolidity==solidPlayer){
        tileMazeMan.takeHit();
    }
    else if(tempSolidity==solidSolid||tempSolidity==solidEnemy){

        if(grid[cI][cJ+1].getSolidity()==solidEmpty){
            T.setAnim(animRight);
            dir=RIGHT;
            grid[T.getI()][T.getJ()]=T;
            T.move(grid,dir,cI,cJ);
            grid[T.getI()][T.getJ()]=T;
        }
        else if(grid[cI][cJ-1].getSolidity()==solidEmpty){
            T.setAnim(animLeft);
            dir=LEFT;
            grid[T.getI()][T.getJ()]=T;
            T.move(grid,dir,cI,cJ);
            grid[T.getI()][T.getJ()]=T;
        }
        else if(grid[cI-1][cJ].getSolidity()==solidEmpty){
            T.setAnim(animUp);
            dir=UP;
            grid[T.getI()][T.getJ()]=T;
            T.move(grid,dir,cI,cJ);
            grid[T.getI()][T.getJ()]=T;
        }
        else if(grid[cI+1][cJ].getSolidity()==solidEmpty){
            T.setAnim(animDown);
            dir=DOWN;
            grid[T.getI()][T.getJ()]=T;
            T.move(grid,dir,cI,cJ);
            grid[T.getI()][T.getJ()]=T;
        }


    }
    else if(tempSolidity==solidAttack){
        mmEffectEx(&swordSound);
        T.takeHit();
        T.move(grid,dir,cI,cJ);
        grid[T.getI()][T.getJ()]=T;
    }

}

//moves the player and performs the relevant collision detection etc
void actorMove(tile grid[][gridJ],player &T, int direction){

    int tempID=0; int tempSolidity=0;
    T.incCounter();
    if(direction==UP){
        T.setAnim(animManUp);
        tempID=grid[T.getI()-1][T.getJ()].getID();
        tempSolidity=grid[T.getI()-1][T.getJ()].getSolidity();
    }
    else if(direction==DOWN){
        T.setAnim(animManDown);
        tempID=grid[T.getI()+1][T.getJ()].getID();
        tempSolidity=grid[T.getI()+1][T.getJ()].getSolidity();
    }
    else if(direction==LEFT){
        T.setAnim(animManLeft);
        tempID=grid[T.getI()][T.getJ()-1].getID();
        tempSolidity=grid[T.getI()][T.getJ()-1].getSolidity();
    }
    else if(direction==RIGHT){
        T.setAnim(animManRight);
        tempID=grid[T.getI()][T.getJ()+1].getID();
        tempSolidity=grid[T.getI()][T.getJ()+1].getSolidity();
    }
    //enforce the updated animation
    grid[T.getI()][T.getJ()]=T;

    //check for obstacles/objects etc in the direction
    int dir=direction;
    int cI=T.getI();
    int cJ=T.getJ();

    //go there if its empty
    if(tempSolidity==solidEmpty||tempSolidity==solidAttack){
        T.move(grid,dir,cI,cJ);
        grid[T.getI()][T.getJ()]=T;
    }
    else if(tempSolidity==solidEnemy){
        //do nothing, just chill here
    }
    else if(tempID==tileGoldID){
        T.move(grid,dir,cI,cJ);
        grid[T.getI()][T.getJ()]=T;
        mmEffectEx(&keyPickupSound);
        goldAcquired=1;
    }
    else if(tempID==tileEndID){
        grid[tileMazeMan.getI()][tileMazeMan.getJ()]=tileEmpty;
        victBool=true;
        animateItem(grid, tileEnd, 0.3,ladderSound);
        mmEffectEx(&ladderSound);
        tileEnd.defaultSprite();
        grid[tileEnd.getI()][tileEnd.getJ()]=tileEnd;
        printScreen(grid);
    }


}

//used for getting the user direction for the MazeMan
void getUserInput(tile grid[][gridJ],int &direction, int &prevDir, int held){

        int currentI=tileMazeMan.getI();
        int currentJ=tileMazeMan.getJ();

       //get user input, L+R is a cheat
        if((held & KEY_R)&&(held & KEY_L)){
            int waste=0;

            findRoute(grid,currentI,currentJ,tileEnd.getI(),tileEnd.getJ(),waste,direction);
		}
		else if(held & KEY_UP){
		    if(direction!=STOP){prevDir=direction;}
		    if(prevDir!=UP){tileMazeMan.cancelAttacking();}   //for the sword swipe, the brief delay only counts if you dont change direction
            direction=UP;
		}
		else if(held & KEY_DOWN){
		    if(direction!=STOP){prevDir=direction;}
		    if(prevDir!=DOWN){tileMazeMan.cancelAttacking();}
            direction=DOWN;
		}
        else if(held & KEY_LEFT){
            if(direction!=STOP){prevDir=direction;}
            if(prevDir!=LEFT){tileMazeMan.cancelAttacking();}
            direction=LEFT;
		}
        else if(held & KEY_RIGHT){
            if(direction!=STOP){prevDir=direction;}
            if(prevDir!=RIGHT){tileMazeMan.cancelAttacking();}
            direction=RIGHT;
		}
		else{
		    if(direction!=STOP){prevDir=direction;}
            direction=STOP;
		}

}

//random movement for characters such as zombies
void randomMovement(tile grid[][gridJ],actor &T, int &direction,int &previousDir, int &steps){
    int workingI=T.getI();
    int workingJ=T.getJ();
    int wastei,wastej;
    //begin random movement
    steps=0;

    int up,down,left,right;
    up=down=left=right=0;

    if(grid[workingI][workingJ+1].getSolidity()==solidEmpty){
        right=1;
    }
    if(grid[workingI][workingJ-1].getSolidity()==solidEmpty){
        left=1;
    }
    if(grid[workingI-1][workingJ].getSolidity()==solidEmpty){
        up=1;
    }
    if(grid[workingI+1][workingJ].getSolidity()==solidEmpty){
        down=1;
    }

    int dirTotal=up+down+left+right;
    if(dirTotal==1){
        if(up==1){
            previousDir=UP;
            direction=UP;
        }
        else if(down==1){
            previousDir=DOWN;
            direction=DOWN;
        }
        else if(left==1){
            previousDir=LEFT;
            direction=LEFT;
        }
        else if(right==1){
            previousDir=RIGHT;
            direction=RIGHT;
        }
    }
    else if(dirTotal==0){
        //a weird glitch when two enemies meet, this sorts it out.
        //hackity hack hack hack :(
        previousDir=direction;
        if(grid[workingI][workingJ+1].getSolidity()==solidEmpty){
            direction=RIGHT;
        }
        else if(grid[workingI][workingJ-1].getSolidity()==solidEmpty){
            direction=LEFT;
        }
        else if(grid[workingI-1][workingJ].getSolidity()==solidEmpty){
            direction=UP;
        }
        else if(grid[workingI+1][workingJ].getSolidity()==solidEmpty){
            direction=DOWN;
        }

    }
    else if(dirTotal!=2){
        //get new direction to try, randomly so that the movement looks more fun
        while(1){

            int randDir = rand() % 4;
            if(up==1&&direction!=DOWN&&randDir==UP){
                previousDir=direction;
                direction=UP;
                break;
            }
            else if(right==1&&direction!=LEFT&&randDir==RIGHT){
                previousDir=direction;
                direction=RIGHT;
                break;
            }

            else if(down==1&&direction!=UP&&randDir==DOWN){
                previousDir=direction;
                direction=DOWN;
                break;
            }
            else if(left==1&&direction!=RIGHT&&randDir==LEFT){
                previousDir=direction;
                direction=LEFT;
                break;
            }

        }//end-while
    }
    else{
        previousDir=direction;
        walkToNextJunct(grid,workingI,workingJ,wastei,wastej,direction,steps);
    }
}

void pullSwitch(tile grid[][gridJ],int switchID){

    item *button;
    item *door;
    item *buttonBeta;
    item *doorBeta;
    int *buttonOn=animEnd;
    int *buttonOff=animEnd;
    int *down=animEnd;
    int *left=animEnd;
    int *right=animEnd;
    int *up=animEnd;
    int *downB=animEnd;
    int *leftB=animEnd;
    int *rightB=animEnd;
    int *upB=animEnd;
    int doorID=tileRDoorID;
    int doorBetaID=tileBlueDoorBetaID;

    if(switchID==tileRedButtonID||switchID==tileRedButtonBetaID){
        doorID=     tileRDoorID;
        button=     &tileRedButton;
        door=       &tileRDoor;

        buttonBeta= &tileRedButtonBeta;
        doorBeta=   &tileRDoorBeta;
        doorBetaID= tileRDoorBetaID;

        buttonOn=   animRButton;
        buttonOff=  animRButtonB;
        down=       animDoorDown;
        left=       animDoorLeft;
        right=      animDoorRight;
        up=         animDoorUp;
        downB=      animDoorDownB;
        leftB=      animDoorLeftB;
        rightB=     animDoorRightB;
        upB=        animDoorUpB;
    }
    else if(switchID==tileGreenButtonID||switchID==tileGreenButtonBetaID){
        doorID=     tileGDoorID;
        button=     &tileGreenButton;
        door=       &tileGDoor;

        buttonBeta= &tileGreenButtonBeta;
        doorBeta=   &tileGDoorBeta;
        doorBetaID= tileGDoorBetaID;

        buttonOn=   animGButton;
        buttonOff=  animGButtonB;
        down=       animDoorGDown;
        left=       animDoorGLeft;
        right=      animDoorGRight;
        up=         animDoorGUp;
        downB=      animDoorGDownB;
        leftB=      animDoorGLeftB;
        rightB=     animDoorGRightB;
        upB=        animDoorGUpB;

    }
    else if(switchID==tileBlueButtonID||switchID==tileBlueButtonBetaID){
        doorID=     tileBlueDoorID;
        door=       &tileBlueDoor;
        button=     &tileBlueButton;

        buttonBeta= &tileBlueButtonBeta;
        doorBeta=   &tileBlueDoorBeta;
        doorBetaID= tileBlueDoorBetaID;

        buttonOn=   animBlueButton;
        buttonOff=  animBlueButtonB;
        down=       animDoorBlueDown;
        left=       animDoorBlueLeft;
        right=      animDoorBlueRight;
        up=         animDoorBlueUp;
        downB=      animDoorBlueDownB;
        leftB=      animDoorBlueLeftB;
        rightB=     animDoorBlueRightB;
        upB=        animDoorBlueUpB;
    }
    else if(switchID==tileOrangeButtonID||switchID==tileOrangeButtonBetaID){
        doorID=     tileOrangeDoorID;
        door=       &tileOrangeDoor;
        button=     &tileOrangeButton;

        buttonBeta= &tileOrangeButtonBeta;
        doorBeta=   &tileOrangeDoorBeta;
        doorBetaID= tileOrangeDoorBetaID;

        buttonOn=   animOrangeButton;
        buttonOff=  animOrangeButtonB;
        down=       animDoorOrangeDown;
        left=       animDoorOrangeLeft;
        right=      animDoorOrangeRight;
        up=         animDoorOrangeUp;
        downB=      animDoorOrangeDownB;
        leftB=      animDoorOrangeLeftB;
        rightB=     animDoorOrangeRightB;
        upB=        animDoorOrangeUpB;
    }

    if(grid[door->getI()][door->getJ()].getID()!=doorID){
        mmEffectEx(&doorStuck);
        return;
    }

    if(doorBeta->getI()!=-1&&doorBeta->getJ()!=-1){
        if(grid[doorBeta->getI()][doorBeta->getJ()].getID()!=doorBetaID){
            mmEffectEx(&doorStuck);
            return;
        }
    }

    if(button->getStatus()==false){

        button->setAnim(buttonOn);
        button->defaultSprite();
        button->setStatus(true);

        if(buttonBeta->getI()!=-1&&buttonBeta->getJ()!=-1){
            buttonBeta->setAnim(buttonOn);
            buttonBeta->defaultSprite();
            buttonBeta->setStatus(true);
        }

        if(door->getAnim()==downB){door->setAnim(down);}
        else if(door->getAnim()==leftB){door->setAnim(left);}
        else if(door->getAnim()==rightB){door->setAnim(right);}
        else if(door->getAnim()==upB){door->setAnim(up);}
        door->defaultSprite();

        if(doorBeta->getI()!=-1&&doorBeta->getJ()!=-1){
            if(doorBeta->getAnim()==downB){doorBeta->setAnim(down);}
            else if(doorBeta->getAnim()==leftB){doorBeta->setAnim(left);}
            else if(doorBeta->getAnim()==rightB){doorBeta->setAnim(right);}
            else if(doorBeta->getAnim()==upB){doorBeta->setAnim(up);}
            doorBeta->defaultSprite();
        }

        //pulls the lever
        mmEffectEx(&keyPickupSound);
        if(buttonBeta->getI()!=-1&&buttonBeta->getJ()!=-1){
            animateDual(grid,button,buttonBeta, 0.1);
        }
        else{
            animateItem(grid,button, 0.1);
        }
        secondDelay(0.5);

        //opens the door, and that kinda jazz
        if(doorBeta->getI()!=-1&&doorBeta->getJ()!=-1){
            animateDual(grid,door,doorBeta, 0.5, doorOpenSound);
            doorBeta->setSolidity(solidEmpty);
            grid[doorBeta->getI()][doorBeta->getJ()]=*doorBeta;
            door->setSolidity(solidEmpty);
            grid[door->getI()][door->getJ()]=*door;
        }
        else{
            animateItem(grid,door,0.5, doorOpenSound);
        }
        door->setSolidity(solidEmpty);
        grid[door->getI()][door->getJ()]=*door;

        return;

    }//end button check
    else if(button->getStatus()==true){

        button->setAnim(buttonOff);
        button->defaultSprite();
        button->setStatus(false);

        if(buttonBeta->getI()!=-1&&buttonBeta->getJ()!=-1){
            buttonBeta->setAnim(buttonOff);
            buttonBeta->defaultSprite();
            buttonBeta->setStatus(false);
        }

        if(door->getAnim()==down){door->setAnim(downB);}
        else if(door->getAnim()==left){door->setAnim(leftB);}
        else if(door->getAnim()==right){door->setAnim(rightB);}
        else if(door->getAnim()==up){door->setAnim(upB);}
        door->defaultSprite();

        if(doorBeta->getI()!=-1&&doorBeta->getJ()!=-1){
            if(doorBeta->getAnim()==down){doorBeta->setAnim(downB);}
            else if(doorBeta->getAnim()==left){doorBeta->setAnim(leftB);}
            else if(doorBeta->getAnim()==right){doorBeta->setAnim(rightB);}
            else if(doorBeta->getAnim()==up){doorBeta->setAnim(upB);}
            doorBeta->defaultSprite();
        }

        //pulls the lever
        mmEffectEx(&keyPickupSound);
        if(buttonBeta->getI()!=-1&&buttonBeta->getJ()!=-1){
            animateDual(grid,button,buttonBeta, 0.1);
        }
        else{
            animateItem(grid,button, 0.1);
        }
        secondDelay(0.5);


        //opens the door, and that kinda jazz
        if(doorBeta->getI()!=-1&&doorBeta->getJ()!=-1){
            animateDual(grid,door,doorBeta,0.5, doorOpenSound);
            doorBeta->setSolidity(solidItem);
            grid[doorBeta->getI()][doorBeta->getJ()]=*doorBeta;
        }
        else{
            animateItem(grid,door,0.5, doorOpenSound);
        }
        door->setSolidity(solidItem);
        grid[door->getI()][door->getJ()]=*door;

        return;


    }//end button check

}

int pauseGame(){
    mmEffectEx(&ladderSound);
    int position=0;
    int returnval=1;

    //make sure you're not holding start!
    while(1){
        scanKeys();
        int held = keysUp();
        if((held & KEY_START)){
            break;
        }
    }
    consoleClear();

    while(1){
        iprintf("\x1b[1;16HPoints: %7d",totalScore);
        scanKeys();
        int held = keysHeld();

        if(held & KEY_A){
            if(position==0){
                break;
            }
            else if(position==1){
                //quitters get no points!
                totalScore=0;
                returnval=-1;
                break;
            }
        }

        if(held & KEY_START){
            break;
        }

        if(held & KEY_UP){ if(position!=0){position--; mmEffectEx(&ladderSound);}}
        else if(held & KEY_DOWN){ if(position!=1){position++; mmEffectEx(&ladderSound);}}

        iprintf("\x1b[3;5HGame Paused");
        iprintf("\x1b[4;5H-----------");
        iprintf("\x1b[5;5HContinue");
        iprintf("\x1b[6;5HQuit");

        if(position==0){
            iprintf("\x1b[5;3H>");
            iprintf("\x1b[6;3H ");
        }
        else if(position==1){
            iprintf("\x1b[5;3H ");
            iprintf("\x1b[6;3H>");
        }

        secondDelay(0.1);
        frameCounter++;

    }
    return returnval;

}

int deathOccured(){
    mmEffectEx(&zombieMoan);
    setFont();
    showPicture(picDeath);

    int position=0;
    int returnval=1;
    int cost=0;
    if(mode==EASY){cost=tryEasy;}
    else if(mode==NORMAL){cost=tryNormal;}
    else if(mode==HARD){cost=tryHard;}

    secondDelay(1);
    iprintf("\x1b[1;16HPoints: %7d",totalScore);
    if(totalScore<cost){
        if(deathType==typeZomb){iprintf("\x1b[3;2HYou were killed by a Zombie");}
        else{iprintf("\x1b[3;2HYou were crushed by a Rock");}

        iprintf("\x1b[5;2HYou need %d points,", cost);
        iprintf("\x1b[6;2Hyou do not have enough to");
        iprintf("\x1b[7;2Htry again");
        iprintf("\x1b[9;2H---------------------------");
        iprintf("\x1b[10;1H> Accept Your Fate");

        returnval = -1;
        waitAtoContinue();
    }//end easy failed death
    else{
        while(1){
            iprintf("\x1b[1;16HPoints: %7d",totalScore);
            scanKeys();
            int held = keysHeld();

            if(held & KEY_A){
                if(position==0){

                    while(1){
                        scanKeys();
                        int held = keysUp();
                        if((held & KEY_A)){
                            break;
                        }
                    }
                    returnval=0;
                    break;
                }
                else if(position==1){
                    while(1){
                        scanKeys();
                        int held = keysUp();
                        if((held & KEY_A)){
                            break;
                        }
                    }
                    returnval=-1;
                    break;
                }
            }

            if(held & KEY_UP){ if(position!=0){position--; mmEffectEx(&ladderSound);}}
            else if(held & KEY_DOWN){ if(position!=1){position++; mmEffectEx(&ladderSound);}}

            if(deathType==typeZomb){iprintf("\x1b[3;2HYou were killed by a Zombie");}
                            else{iprintf("\x1b[3;2HYou were crushed by a Rock");}

            iprintf("\x1b[4;2H---------------------------");
            iprintf("\x1b[6;3HTry Again");
            iprintf("\x1b[7;3HAccept Your Fate");

            if(position==0){
                iprintf("\x1b[6;1H>");
                iprintf("\x1b[7;1H ");
                iprintf("\x1b[10;3H(Costs %4d Points)       ",cost);
            }
            else if(position==1){
                iprintf("\x1b[6;1H ");
                iprintf("\x1b[7;1H>");
                iprintf("\x1b[10;3H(Quit and Save Points)     ");
            }

            secondDelay(0.1);
            frameCounter++;

        }
    }//end normal death
    setScreenForPlay();
    consoleClear();

    return returnval;


}

void checkSwitch(tile grid[][gridJ], int held,  int I, int J, int dir, int buttonID, int button2ID){

    //checks for red button press
    if( (held & KEY_A) &&(
        (dir==RIGHT&&grid[I][J+1].getID()==buttonID)||
        (dir==LEFT&&grid[I][J-1].getID()==buttonID)||
        (dir==UP&&grid[I-1][J].getID()==buttonID)||
        (dir==DOWN&&grid[I+1][J].getID()==buttonID) )){

            pullSwitch(grid,buttonID);
    }//end redbutton check

        //checks for red button press
    if( (held & KEY_A) &&(
        (dir==RIGHT&&grid[I][J+1].getID()==button2ID)||
        (dir==LEFT&&grid[I][J-1].getID()==button2ID)||
        (dir==UP&&grid[I-1][J].getID()==button2ID)||
        (dir==DOWN&&grid[I+1][J].getID()==button2ID) )){

            pullSwitch(grid,button2ID);
    }//end redbutton check

}

//check actions for button presses and the like
int checkActions(tile grid[][gridJ], int currentI, int currentJ, int direction, int previousDir, int held){

    //pauses the game
    if(held & KEY_START){
        int val = pauseGame();
        secondDelay(0.1);
        consoleClear();
        mmEffectEx(&ladderSound);

        while(1){
            scanKeys();
            int held = keysDown();
            if(!(held & KEY_A)&&!(held & KEY_START)){
                break;
            }
        }

        return val;
    }

    checkSwitch(grid,held,currentI,currentJ, previousDir,tileRedButtonID,tileRedButtonBetaID);
    checkSwitch(grid,held,currentI,currentJ, previousDir,tileGreenButtonID,tileGreenButtonBetaID);
    checkSwitch(grid,held,currentI,currentJ, previousDir,tileBlueButtonID,tileBlueButtonBetaID);
    checkSwitch(grid,held,currentI,currentJ, previousDir,tileOrangeButtonID,tileOrangeButtonBetaID);

    int facingDirection=direction;
    if(direction==STOP){facingDirection=previousDir;}

    //DEAL WITH ATTACKS!
    if((held & KEY_B)){

        //allows you to attack 3 times per second
        if(tileMazeMan.attackReady()==20){

            mmEffectEx(&ladderSound);
            tileMazeMan.attackUsed();

            if(facingDirection==LEFT){tileMazeMan.setSprite(spriteManAttLeft);}
            if(facingDirection==RIGHT){tileMazeMan.setSprite(spriteManAttRight);}
            if(facingDirection==UP){tileMazeMan.setSprite(spriteManAttUp);}
            if(facingDirection==DOWN){tileMazeMan.setSprite(spriteManAttDown);}

            grid[tileMazeMan.getI()][tileMazeMan.getJ()]=tileMazeMan;
            printScreen(grid);
            swiWaitForVBlank();
            swiWaitForVBlank();
            swiWaitForVBlank();

            int ID=-1;
            if(facingDirection==RIGHT&&grid[currentI][currentJ+1].getSolidity()==solidEnemy){
                ID=grid[currentI][currentJ+1].getID();
            }
            else if(facingDirection==LEFT&&grid[currentI][currentJ-1].getSolidity()==solidEnemy){
                ID=grid[currentI][currentJ-1].getID();
            }
            else if(facingDirection==UP&&grid[currentI-1][currentJ].getSolidity()==solidEnemy){
                ID=grid[currentI-1][currentJ].getID();
            }
            else if(facingDirection==DOWN&&grid[currentI+1][currentJ].getSolidity()==solidEnemy){
                ID=grid[currentI+1][currentJ].getID();
            }


            if(ID!=-1){
                if(ID==tileZombieAID){
                    tileZombieA.takeHit();
                    mmEffectEx(&swordSound);
                }
                else if(ID==tileZombieBID){
                    tileZombieB.takeHit();
                    mmEffectEx(&swordSound);
                }
                else if(ID==tileZombieCID){
                    tileZombieC.takeHit();
                    mmEffectEx(&swordSound);
                }
                else if(ID==tileZombieDID){
                    tileZombieD.takeHit();
                    mmEffectEx(&swordSound);
                }
                else if(ID==tileZombieEID){
                    tileZombieE.takeHit();
                    mmEffectEx(&swordSound);
                }
            }
        }

    }//end ENEMY CHECK
    return 1;
}

void mazeManAttack(tile grid[][gridJ],int mazeManDir, int previousDir){

    int currentI = tileMazeMan.getI();
    int currentJ = tileMazeMan.getJ();

    //deals with mazeman attacks
    if (tileMazeMan.getAttacking()==true){
        int checkingDirection=mazeManDir;
        if(mazeManDir==STOP){checkingDirection=previousDir;}
        int solidity=0;
        int workingI=currentI;
        int workingJ=currentJ;

        if(checkingDirection==RIGHT){workingJ++;}
        else if(checkingDirection==LEFT){workingJ--;}
        else if(checkingDirection==DOWN){workingI++;}
        else if(checkingDirection==UP){workingI--;}

        solidity=grid[workingI][workingJ].getSolidity();
        if(solidity==solidEmpty){grid[workingI][workingJ].setSolidity(solidAttack);}
    }

}

void zombieMovement(tile grid[][gridJ],actor &T, int &dir, int &prevDir, mm_sound_effect sfx, int manDir){

    //zombie cannot move if it has not been placed
    if((T.getI()==-1&&T.getJ()==-1)||dir==STOP){return;}

    int distance=0;
    int aggrodistance=15;
    int speed=25;

    if(mode==NORMAL){
        speed=20;
    }
    else if(mode==HARD){
        aggrodistance=25;
        speed=15;
    }
    prevDir=dir;

    int fastSpeed=speed;
    int slowSpeed=speed+5;

    if(T.getHealth()==3){
        fastSpeed=speed+5;
        slowSpeed=speed+10;
    }
    else if(T.getHealth()==2){
        fastSpeed=speed+10;
        slowSpeed=speed+15;
    }
    else{
        fastSpeed=speed+15;
        slowSpeed=speed+20;
    }

    int tempDir=dir;

    int routestatus = findRoute(grid,T.getI(),T.getJ(),tileMazeMan.getI(),tileMazeMan.getJ(),distance,tempDir);

    if(distance>aggrodistance||routestatus==-1){

        T.setScream(false);
        randomMovement(grid,T,dir,prevDir,distance);
        T.setDelay(slowSpeed);

    }
    else{
        if(T.getScream()==false){
            mmEffectEx(&sfx);
            T.setScream(true);
        }
        dir=tempDir;
        T.setDelay(fastSpeed);
    }

    actorMove(grid,T,dir,manDir,sfx);


}


void showZombie(tile grid[][gridJ],int level){

    if(tileZombieA.getHealth()!=0&&tileZombieA.getI()!=-1&&tileZombieA.getJ()!=-1){
        tileZombieA.setSolidity(solidEnemy);
        grid[tileZombieA.getI()][tileZombieA.getJ()]=tileZombieA;
    }

    if(tileZombieB.getHealth()!=0&&tileZombieB.getI()!=-1&&tileZombieB.getJ()!=-1){
        tileZombieB.setSolidity(solidEnemy);
        grid[tileZombieB.getI()][tileZombieB.getJ()]=tileZombieB;
    }

    if(tileZombieC.getHealth()!=0&&tileZombieC.getI()!=-1&&tileZombieC.getJ()!=-1){
        tileZombieC.setSolidity(solidEnemy);
        grid[tileZombieC.getI()][tileZombieC.getJ()]=tileZombieC;
    }

    if(tileZombieD.getHealth()!=0&&tileZombieD.getI()!=-1&&tileZombieD.getJ()!=-1){
        tileZombieD.setSolidity(solidEnemy);
        grid[tileZombieD.getI()][tileZombieD.getJ()]=tileZombieD;
    }

    if(tileZombieE.getHealth()!=0&&tileZombieE.getI()!=-1&&tileZombieE.getJ()!=-1){
        tileZombieE.setSolidity(solidEnemy);
        grid[tileZombieE.getI()][tileZombieE.getJ()]=tileZombieE;
    }


}

void showZombieDead(tile grid[][gridJ],int level){

    if(level==5){
        if(tileZombieA.getHealth()==0&&tileZombieA.getI()!=-1&&tileZombieA.getJ()!=-1){
            grid[tileZombieA.getI()][tileZombieA.getJ()].setSolidity(solidEmpty);
            grid[tileZombieA.getI()][tileZombieA.getJ()].setSprite(spriteZombieDead);
        }
    }
    else{
        if(tileZombieA.getHealth()==0&&tileZombieA.getI()!=-1&&tileZombieA.getJ()!=-1){
            tileZombieA.setSolidity(solidEmpty);
            tileZombieA.setSprite(spriteZombieDead);
            grid[tileZombieA.getI()][tileZombieA.getJ()]=tileZombieA;
        }
    }
    if(tileZombieB.getHealth()==0&&tileZombieB.getI()!=-1&&tileZombieB.getJ()!=-1){
        tileZombieB.setSolidity(solidEmpty);
        tileZombieB.setSprite(spriteZombieDead);
        grid[tileZombieB.getI()][tileZombieB.getJ()]=tileZombieB;
    }

    if(tileZombieC.getHealth()==0&&tileZombieC.getI()!=-1&&tileZombieC.getJ()!=-1){
        tileZombieC.setSolidity(solidEmpty);
        tileZombieC.setSprite(spriteZombieDead);
        grid[tileZombieC.getI()][tileZombieC.getJ()]=tileZombieC;
    }

    if(tileZombieD.getHealth()==0&&tileZombieD.getI()!=-1&&tileZombieD.getJ()!=-1){
        tileZombieD.setSolidity(solidEmpty);
        tileZombieD.setSprite(spriteZombieDead);
        grid[tileZombieD.getI()][tileZombieD.getJ()]=tileZombieD;
    }

    if(tileZombieE.getHealth()==0&&tileZombieE.getI()!=-1&&tileZombieE.getJ()!=-1){
        tileZombieE.setSolidity(solidEmpty);
        tileZombieE.setSprite(spriteZombieDead);
        grid[tileZombieE.getI()][tileZombieE.getJ()]=tileZombieE;
    }



}


//level two, 2 zombies, 1 door, gold
int populateLevelTwo(tile grid[][gridJ]){

    stack<int> deadEndCopy;
    stack<int> deadEnd;
    int ti=0,tj=0,dir,steps=0, newsteps=0;
    int largestSteps=0;
    int up,down,left,right;

    tileMazeMan.setI(1);
    tileMazeMan.setJ(1);
    grid[1][1].setSolidity(solidReserved);
    int deadEndStepCount=0;
    int workingI=0;
    int workingJ=0;
    int takenDirection=STOP;
    int previousTakenDirection=STOP;
    int stepsTilJunct=0;

    //collect all the dead end pieces of the maze!
    for(int i=0;i<gridI;i++){
        for(int j=0;j<gridJ;j++){
            if(grid[i][j].getID()==tileEmptyID){

                findAdjacentEmptyTiles(grid,i,j,up,down,left,right);
                int direction=0;
                if(up==1&&down==0&&left==0&&right==0){
                    direction=UP;
                }
                else if(up==0&&down==1&&left==0&&right==0){
                    direction=DOWN;
                }
                else if(up==0&&down==0&&left==1&&right==0){
                    direction=LEFT;
                }
                else if(up==0&&down==0&&left==0&&right==1){
                    direction=RIGHT;
                }

                if((up+down+left+right)==1){
                    walkToNextJunct(grid,i,j,workingI,workingJ,direction,deadEndStepCount);
                    if(deadEndStepCount>3){
                        deadEnd.push(i);
                        deadEnd.push(j);
                    }
                }
            }
        }
    }//collect dead ends
    if(deadEnd.size()<12){return -1;}

    //gets the IJ location of the dead end!
    deadEndCopy = deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        findRoute(grid,tileMazeMan.getI(),tileMazeMan.getJ(),ti,tj,steps,dir);
        if(steps>largestSteps){
            largestSteps=steps;
            workingI=ti;
            workingJ=tj;
            tileEnd.setI(ti);
            tileEnd.setJ(tj);
        }
    }//found the dead end to place the End-Of-The-Maze
    grid[tileEnd.getI()][tileEnd.getJ()].setSolidity(solidReserved);

    findAdjacentEmptyTiles(grid,tileEnd.getI(),tileEnd.getJ(),up,down,left,right);
    takenDirection=STOP;
    previousTakenDirection=STOP;

    if(up==0&&down==0&&left==0&&right==1){
        previousTakenDirection=takenDirection;
        tileZombieB.setI(workingI);
        tileZombieB.setJ(workingJ+1);
        takenDirection=RIGHT;
    }
    else if(up==0&&down==0&&left==1&&right==0){
        previousTakenDirection=takenDirection;
        tileZombieB.setI(workingI);
        tileZombieB.setJ(workingJ-1);
        takenDirection=LEFT;
    }
    else if(up==0&&down==1&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        tileZombieB.setI(workingI+1);
        tileZombieB.setJ(workingJ);
        takenDirection=DOWN;
    }
    else if(up==1&&down==0&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        tileZombieB.setI(workingI-1);
        tileZombieB.setJ(workingJ);
        takenDirection=UP;
    }
    grid[tileZombieB.getI()][tileZombieB.getJ()].setSolidity(solidReserved);

    stepsTilJunct=0;
    walkToNextJunct(grid,tileEnd.getI(),tileEnd.getJ(),workingI,workingJ,takenDirection,stepsTilJunct);
    findRoute(grid,workingI,workingJ,tileEnd.getI(),tileEnd.getJ(),steps,dir);
    if(dir==UP){
        workingI--;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGDown);
    }
    else if(dir==DOWN){
        workingI++;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGUp);
    }
    else if(dir==LEFT){
        workingJ--;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGRight);
    }
    else if(dir==RIGHT){
        workingJ++;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGLeft);
    }
    grid[tileGDoor.getI()][tileGDoor.getJ()].setSolidity(solidReserved);

    newsteps=steps=0;
    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()==solidEmpty){
            findRoute(grid,workingI,workingJ,ti,tj,steps,dir);
            if(steps>newsteps&&grid[ti][tj].getSolidity()!=solidReserved){
                newsteps=steps;
                tileGreenButton.setI(ti);
                tileGreenButton.setJ(tj);
            }
        }
    }
    grid[tileGreenButton.getI()][tileGreenButton.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileGold.setI(ti);
            tileGold.setJ(tj);
        }
    }
    grid[tileGold.getI()][tileGold.getJ()].setSolidity(solidReserved);

    findAdjacentEmptyTiles(grid,tileGold.getI(),tileGold.getJ(),up,down,left,right);
    takenDirection=STOP;
    previousTakenDirection=STOP;

    if(up==0&&down==0&&left==0&&right==1){
        previousTakenDirection=takenDirection;
        takenDirection=RIGHT;
    }
    else if(up==0&&down==0&&left==1&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=LEFT;
    }
    else if(up==0&&down==1&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=DOWN;
    }
    else if(up==1&&down==0&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=UP;
    }

    stepsTilJunct=0;
    walkToNextJunct(grid,tileGold.getI(),tileGold.getJ(),workingI,workingJ,takenDirection,stepsTilJunct);
    findRoute(grid,workingI,workingJ,tileGold.getI(),tileGold.getJ(),steps,dir);
    if(dir==UP){
        workingI--;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueDown);
    }
    else if(dir==DOWN){
        workingI++;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueUp);
    }
    else if(dir==LEFT){
        workingJ--;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueRight);
    }
    else if(dir==RIGHT){
        workingJ++;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueLeft);
    }
    grid[tileBlueDoor.getI()][tileBlueDoor.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileZombieA.setI(ti);
            tileZombieA.setJ(tj);
        }
    }
    grid[tileZombieA.getI()][tileZombieA.getJ()].setSolidity(solidReserved);

    newsteps=steps=0;
    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()==solidEmpty){
            findRoute(grid,workingI,workingJ,ti,tj,steps,dir);
            if(steps>newsteps&&grid[ti][tj].getSolidity()!=solidReserved){
                newsteps=steps;
                tileBlueButton.setI(ti);
                tileBlueButton.setJ(tj);
            }
        }
    }
    grid[tileBlueButton.getI()][tileBlueButton.getJ()].setSolidity(solidReserved);

    grid[tileZombieA.getI()][tileZombieA.getJ()]=tileZombieA;
    grid[tileGold.getI()][tileGold.getJ()]=tileGold;
    grid[tileBlueButton.getI()][tileBlueButton.getJ()]=tileBlueButton;
    grid[tileGreenButton.getI()][tileGreenButton.getJ()]=tileGreenButton;
    grid[tileMazeMan.getI()][tileMazeMan.getJ()]=tileMazeMan;
    grid[tileEnd.getI()][tileEnd.getJ()]=tileEnd;
    return 1;

}

//level two, 3 zombies, 2 door, gold
int populateLevelThree(tile grid[][gridJ]){

    stack<int> deadEndCopy;
    stack<int> deadEnd;
    int ti=0,tj=0,dir,steps=0, newsteps=0;
    int largestSteps=0;
    int up,down,left,right;

    tileMazeMan.setI(1);
    tileMazeMan.setJ(1);
    grid[1][1].setSolidity(solidReserved);
    int deadEndStepCount=0;
    int workingI=0;
    int workingJ=0;
    int takenDirection=STOP;
    int previousTakenDirection=STOP;
    int stepsTilJunct=0;

    //collect all the dead end pieces of the maze!
    for(int i=0;i<gridI;i++){
        for(int j=0;j<gridJ;j++){
            if(grid[i][j].getID()==tileEmptyID){

                findAdjacentEmptyTiles(grid,i,j,up,down,left,right);
                int direction=0;
                if(up==1&&down==0&&left==0&&right==0){
                    direction=UP;
                }
                else if(up==0&&down==1&&left==0&&right==0){
                    direction=DOWN;
                }
                else if(up==0&&down==0&&left==1&&right==0){
                    direction=LEFT;
                }
                else if(up==0&&down==0&&left==0&&right==1){
                    direction=RIGHT;
                }

                if((up+down+left+right)==1){
                    walkToNextJunct(grid,i,j,workingI,workingJ,direction,deadEndStepCount);
                    if(deadEndStepCount>3){
                        deadEnd.push(i);
                        deadEnd.push(j);
                    }
                }
            }
        }
    }//collect dead ends
    if(deadEnd.size()<12){return -1;}

    //gets the IJ location of the dead end!
    deadEndCopy = deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        findRoute(grid,tileMazeMan.getI(),tileMazeMan.getJ(),ti,tj,steps,dir);
        if(steps>largestSteps){
            largestSteps=steps;
            workingI=ti;
            workingJ=tj;
            tileEnd.setI(ti);
            tileEnd.setJ(tj);
        }
    }//found the dead end to place the End-Of-The-Maze
    grid[tileEnd.getI()][tileEnd.getJ()].setSolidity(solidReserved);

    findAdjacentEmptyTiles(grid,tileEnd.getI(),tileEnd.getJ(),up,down,left,right);
    takenDirection=STOP;
    previousTakenDirection=STOP;

    if(up==0&&down==0&&left==0&&right==1){
        previousTakenDirection=takenDirection;
        tileZombieB.setI(workingI);
        tileZombieB.setJ(workingJ+1);
        takenDirection=RIGHT;
    }
    else if(up==0&&down==0&&left==1&&right==0){
        previousTakenDirection=takenDirection;
        tileZombieB.setI(workingI);
        tileZombieB.setJ(workingJ-1);
        takenDirection=LEFT;
    }
    else if(up==0&&down==1&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        tileZombieB.setI(workingI+1);
        tileZombieB.setJ(workingJ);
        takenDirection=DOWN;
    }
    else if(up==1&&down==0&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        tileZombieB.setI(workingI-1);
        tileZombieB.setJ(workingJ);
        takenDirection=UP;
    }
    grid[tileZombieB.getI()][tileZombieB.getJ()].setSolidity(solidReserved);

    stepsTilJunct=0;
    walkToNextJunct(grid,tileEnd.getI(),tileEnd.getJ(),workingI,workingJ,takenDirection,stepsTilJunct);
    findRoute(grid,workingI,workingJ,tileEnd.getI(),tileEnd.getJ(),steps,dir);
    if(dir==UP){
        workingI--;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGDown);
    }
    else if(dir==DOWN){
        workingI++;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGUp);
    }
    else if(dir==LEFT){
        workingJ--;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGRight);
    }
    else if(dir==RIGHT){
        workingJ++;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGLeft);
    }
    grid[tileGDoor.getI()][tileGDoor.getJ()].setSolidity(solidReserved);


    newsteps=steps=0;
    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()==solidEmpty){
            findRoute(grid,workingI,workingJ,ti,tj,steps,dir);
            if(steps>newsteps&&grid[ti][tj].getSolidity()!=solidReserved){
                newsteps=steps;
                workingI=ti;
                workingJ=tj;
                tileGreenButton.setI(ti);
                tileGreenButton.setJ(tj);
            }
        }
    }
    grid[tileGreenButton.getI()][tileGreenButton.getJ()].setSolidity(solidReserved);

    findAdjacentEmptyTiles(grid,tileGreenButton.getI(),tileGreenButton.getJ(),up,down,left,right);
    takenDirection=STOP;
    previousTakenDirection=STOP;

    if(up==0&&down==0&&left==0&&right==1){
        previousTakenDirection=takenDirection;
        tileZombieC.setI(workingI);
        tileZombieC.setJ(workingJ+1);
        takenDirection=RIGHT;
    }
    else if(up==0&&down==0&&left==1&&right==0){
        previousTakenDirection=takenDirection;
        tileZombieC.setI(workingI);
        tileZombieC.setJ(workingJ-1);
        takenDirection=LEFT;
    }
    else if(up==0&&down==1&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        tileZombieC.setI(workingI+1);
        tileZombieC.setJ(workingJ);
        takenDirection=DOWN;
    }
    else if(up==1&&down==0&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        tileZombieC.setI(workingI-1);
        tileZombieC.setJ(workingJ);
        takenDirection=UP;
    }
    grid[tileZombieC.getI()][tileZombieC.getJ()].setSolidity(solidReserved);

    stepsTilJunct=0;
    walkToNextJunct(grid,tileGreenButton.getI(),tileGreenButton.getJ(),workingI,workingJ,takenDirection,stepsTilJunct);
    findRoute(grid,workingI,workingJ,tileGreenButton.getI(),tileGreenButton.getJ(),steps,dir);
    if(dir==UP){
        workingI--;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueDown);
    }
    else if(dir==DOWN){
        workingI++;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueUp);
    }
    else if(dir==LEFT){
        workingJ--;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueRight);
    }
    else if(dir==RIGHT){
        workingJ++;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueLeft);
    }
    grid[tileBlueDoor.getI()][tileBlueDoor.getJ()].setSolidity(solidReserved);

    deadEndCopy = deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileBlueButton.setI(ti);
            tileBlueButton.setJ(tj);
            break;
        }
    }
    grid[tileBlueButton.getI()][tileBlueButton.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileGold.setI(ti);
            tileGold.setJ(tj);
        }
    }
    grid[tileGold.getI()][tileGold.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileZombieA.setI(ti);
            tileZombieA.setJ(tj);
        }
    }
    grid[tileZombieA.getI()][tileZombieA.getJ()].setSolidity(solidReserved);

    grid[tileBlueDoor.getI()][tileBlueDoor.getJ()]=tileBlueDoor;
    grid[tileBlueButton.getI()][tileBlueButton.getJ()]=tileBlueButton;
    grid[tileZombieA.getI()][tileZombieA.getJ()]=tileZombieA;
    grid[tileZombieB.getI()][tileZombieB.getJ()]=tileZombieB;
    grid[tileZombieC.getI()][tileZombieC.getJ()]=tileZombieC;
    grid[tileGold.getI()][tileGold.getJ()]=tileGold;
    grid[tileGreenButton.getI()][tileGreenButton.getJ()]=tileGreenButton;
    grid[tileMazeMan.getI()][tileMazeMan.getJ()]=tileMazeMan;
    grid[tileEnd.getI()][tileEnd.getJ()]=tileEnd;
    return 1;

}

//level one, 1 door, 1 zombie, gold
int populateLevelOne(tile grid[][gridJ]){

    stack<int> deadEndCopy;
    stack<int> deadEnd;
    int ti=0,tj=0,dir,steps=0, newsteps=0;
    int largestSteps=0;
    int up,down,left,right;

    tileMazeMan.setI(1);
    tileMazeMan.setJ(1);
    grid[1][1].setSolidity(solidReserved);
    int deadEndStepCount=0;
    int workingI=0;
    int workingJ=0;
    int takenDirection=STOP;
    int previousTakenDirection=STOP;
    int stepsTilJunct=0;

    //collect all the dead end pieces of the maze!
    for(int i=0;i<gridI;i++){
        for(int j=0;j<gridJ;j++){
            if(grid[i][j].getID()==tileEmptyID){

                findAdjacentEmptyTiles(grid,i,j,up,down,left,right);
                int direction=0;
                if(up==1&&down==0&&left==0&&right==0){
                    direction=UP;
                }
                else if(up==0&&down==1&&left==0&&right==0){
                    direction=DOWN;
                }
                else if(up==0&&down==0&&left==1&&right==0){
                    direction=LEFT;
                }
                else if(up==0&&down==0&&left==0&&right==1){
                    direction=RIGHT;
                }

                if((up+down+left+right)==1){
                    walkToNextJunct(grid,i,j,workingI,workingJ,direction,deadEndStepCount);
                    if(deadEndStepCount>3){
                        deadEnd.push(i);
                        deadEnd.push(j);
                    }
                }
            }
        }
    }//collect dead ends
    if(deadEnd.size()<10){return -1;}

    //gets the IJ location of the dead end!
    deadEndCopy = deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        findRoute(grid,tileMazeMan.getI(),tileMazeMan.getJ(),ti,tj,steps,dir);
        if(steps>largestSteps){
            largestSteps=steps;
            tileEnd.setI(ti);
            tileEnd.setJ(tj);
        }
    }//found the dead end to place the End-Of-The-Maze
    grid[tileEnd.getI()][tileEnd.getJ()].setSolidity(solidReserved);

    findAdjacentEmptyTiles(grid,tileEnd.getI(),tileEnd.getJ(),up,down,left,right);
    takenDirection=STOP;
    previousTakenDirection=STOP;

    if(up==0&&down==0&&left==0&&right==1){
        previousTakenDirection=takenDirection;
        takenDirection=RIGHT;
    }
    else if(up==0&&down==0&&left==1&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=LEFT;
    }
    else if(up==0&&down==1&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=DOWN;
    }
    else if(up==1&&down==0&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=UP;
    }

    stepsTilJunct=0;
    walkToNextJunct(grid,tileEnd.getI(),tileEnd.getJ(),workingI,workingJ,takenDirection,stepsTilJunct);
    findRoute(grid,workingI,workingJ,tileEnd.getI(),tileEnd.getJ(),steps,dir);
    if(dir==UP){
        workingI--;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGDown);
    }
    else if(dir==DOWN){
        workingI++;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGUp);
    }
    else if(dir==LEFT){
        workingJ--;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGRight);
    }
    else if(dir==RIGHT){
        workingJ++;
        tileGDoor.setI(workingI);
        tileGDoor.setJ(workingJ);
        tileGDoor.setAnim(animDoorGLeft);
    }
    grid[tileGDoor.getI()][tileGDoor.getJ()].setSolidity(solidReserved);

    newsteps=steps=0;
    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()==solidEmpty){
            findRoute(grid,workingI,workingJ,ti,tj,steps,dir);
            if(steps>newsteps&&grid[ti][tj].getSolidity()!=solidReserved){
                newsteps=steps;
                tileGreenButton.setI(ti);
                tileGreenButton.setJ(tj);
            }
        }
    }
    grid[tileGreenButton.getI()][tileGreenButton.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileGold.setI(ti);
            tileGold.setJ(tj);
        }
    }
    grid[tileGold.getI()][tileGold.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileZombieA.setI(ti);
            tileZombieA.setJ(tj);
        }
    }
    grid[tileZombieA.getI()][tileZombieA.getJ()].setSolidity(solidReserved);

    grid[tileZombieA.getI()][tileZombieA.getJ()]=tileZombieA;
    grid[tileGold.getI()][tileGold.getJ()]=tileGold;
    grid[tileGreenButton.getI()][tileGreenButton.getJ()]=tileGreenButton;
    grid[tileMazeMan.getI()][tileMazeMan.getJ()]=tileMazeMan;
    grid[tileEnd.getI()][tileEnd.getJ()]=tileEnd;
    return 1;

}

//initialise level variables and all
void initLevel(){
    victBool=false;
    deathBool=false;
    tileMazeMan.reset();
    initTiles();
    tileMazeMan.setDelay(5);
}

//adds level Five, crashy crash crash cave in
int populateLevelFive(tile grid[][gridJ]){

    stack<int> deadEndCopy;
    stack<int> deadEnd;
    int ti=0,tj=0,waste,steps=0;
    int largestSteps=0;
    int I=0, J=0;
    int up,down,left,right;

    tileMazeMan.setI(1);
    tileMazeMan.setJ(1);
    grid[1][1]=tileMazeMan;

    //collect all the dead end pieces of the maze!
    for(int i=0;i<gridI;i++){
        for(int j=0;j<gridJ;j++){

            if(grid[i][j].getID()==tileEmptyID){

                findAdjacentEmptyTiles(grid,i,j,up,down,left,right);
                int direction=0;
                if(up==1&&down==0&&left==0&&right==0){
                    direction=UP;
                }
                else if(up==0&&down==1&&left==0&&right==0){
                    direction=DOWN;
                }
                else if(up==0&&down==0&&left==1&&right==0){
                    direction=LEFT;
                }
                else if(up==0&&down==0&&left==0&&right==1){
                    direction=RIGHT;
                }

                if((up+down+left+right)==1){
                        deadEnd.push(i);
                        deadEnd.push(j);
                }

            }
        }
    }//collect dead ends
    if(deadEnd.size()<12){return -1;}

    //gets the IJ location of the dead end!
    deadEndCopy = deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        findRoute(grid,1,1,ti,tj,steps,waste);
        if(steps>largestSteps){
            I=ti;
            J=tj;
            largestSteps=steps;
        }
    }//found the dead end to place the End-Of-The-Maze

    tileEnd.setI(I);
    tileEnd.setJ(J);
    grid[tileEnd.getI()][tileEnd.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;
    steps=largestSteps=0;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        findRoute(grid,1,1,ti,tj,steps,waste);
        if(steps>largestSteps&&grid[ti][tj].getSolidity()!=solidReserved){
            tileGold.setI(ti);
            tileGold.setJ(tj);
            largestSteps=steps;
        }
    }
    grid[tileGold.getI()][tileGold.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;
    steps=largestSteps=0;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        findRoute(grid,1,1,ti,tj,steps,waste);
        if(steps>largestSteps&&grid[ti][tj].getSolidity()!=solidReserved){
            tileZombieA.setI(ti);
            tileZombieA.setJ(tj);
            largestSteps=steps;
        }
    }
    grid[tileZombieA.getI()][tileZombieA.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;

    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileZombieB.setI(ti);
            tileZombieB.setJ(tj);
            largestSteps=steps;
        }
    }
    grid[tileZombieB.getI()][tileZombieB.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileZombieC.setI(ti);
            tileZombieC.setJ(tj);
            largestSteps=steps;
        }
    }
    grid[tileZombieC.getI()][tileZombieC.getJ()].setSolidity(solidReserved);



    grid[tileGold.getI()][tileGold.getJ()]=tileGold;
    grid[tileMazeMan.getI()][tileMazeMan.getJ()]=tileMazeMan;
    grid[tileEnd.getI()][tileEnd.getJ()]=tileEnd;

    return 1;

}

//level four, 5 zombies,1 blue door, 2 orange
int populateLevelFour(tile grid[][gridJ]){

    stack<int> deadEndCopy;
    stack<int> deadEnd;
    int ti=0,tj=0,waste,steps=0;
    int largestSteps=0;
    int I=0, J=0;
    int up,down,left,right;

    tileMazeMan.setI(1);
    tileMazeMan.setJ(1);
    grid[1][1]=tileMazeMan;
    int deadEndStepCount=0;
    int workingI=0;
    int workingJ=0;

    //collect all the dead end pieces of the maze!
    for(int i=0;i<gridI;i++){
        for(int j=0;j<gridJ;j++){

            if(grid[i][j].getID()==tileEmptyID){

                findAdjacentEmptyTiles(grid,i,j,up,down,left,right);
                int direction=0;
                if(up==1&&down==0&&left==0&&right==0){
                    direction=UP;
                }
                else if(up==0&&down==1&&left==0&&right==0){
                    direction=DOWN;
                }
                else if(up==0&&down==0&&left==1&&right==0){
                    direction=LEFT;
                }
                else if(up==0&&down==0&&left==0&&right==1){
                    direction=RIGHT;
                }

                if((up+down+left+right)==1){
                    walkToNextJunct(grid,i,j,workingI,workingJ,direction,deadEndStepCount);
                    if(deadEndStepCount>3){
                        deadEnd.push(i);
                        deadEnd.push(j);
                    }
                }
            }
        }
    }//collect dead ends
    if(deadEnd.size()!=14){return -1;}


    //gets the IJ location of the dead end!
    deadEndCopy = deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        findRoute(grid,1,1,ti,tj,steps,waste);
        if(steps>largestSteps){
            I=ti;
            J=tj;
            largestSteps=steps;
            tileEnd.setI(I);
            tileEnd.setJ(J);
        }
    }//found the dead end to place the End-Of-The-Maze
    grid[tileEnd.getI()][tileEnd.getJ()].setSolidity(solidReserved);


    findAdjacentEmptyTiles(grid,I,J,up,down,left,right);
    int takenDirection=STOP;
    int previousTakenDirection=STOP;

    if(up==0&&down==0&&left==0&&right==1){
        previousTakenDirection=takenDirection;
        takenDirection=RIGHT;
        tileZombieA.setI(I);
        tileZombieA.setJ(J+1);
    }
    else if(up==0&&down==0&&left==1&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=LEFT;
        tileZombieA.setI(I);
        tileZombieA.setJ(J-1);
    }
    else if(up==0&&down==1&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=DOWN;
        tileZombieA.setI(I+1);
        tileZombieA.setJ(J);
    }
    else if(up==1&&down==0&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=UP;
        tileZombieA.setI(I-1);
        tileZombieA.setJ(J);
    }
    grid[tileZombieA.getI()][tileZombieA.getJ()].setSolidity(solidReserved);

    //trace to next junction
    int stepsTilJunct=0;


    walkToNextJunct(grid,tileEnd.getI(),tileEnd.getJ(),workingI,workingJ,takenDirection,stepsTilJunct);
    findRoute(grid,workingI,workingJ,tileEnd.getI(),tileEnd.getJ(),steps,waste);
    if(waste==UP){
        workingI--;
        tileOrangeDoor.setI(workingI);
        tileOrangeDoor.setJ(workingJ);
        tileOrangeDoor.setAnim(animDoorOrangeDown);
    }
    else if(waste==DOWN){
        workingI++;
        tileOrangeDoor.setI(workingI);
        tileOrangeDoor.setJ(workingJ);
        tileOrangeDoor.setAnim(animDoorOrangeUp);
    }
    else if(waste==LEFT){
        workingJ--;
        tileOrangeDoor.setI(workingI);
        tileOrangeDoor.setJ(workingJ);
        tileOrangeDoor.setAnim(animDoorOrangeRight);
    }
    else if(waste==RIGHT){
        workingJ++;
        tileOrangeDoor.setI(workingI);
        tileOrangeDoor.setJ(workingJ);
        tileOrangeDoor.setAnim(animDoorOrangeLeft);
    }//Red door placed!
    grid[tileOrangeDoor.getI()][tileOrangeDoor.getJ()].setSolidity(solidReserved);

    int newsteps=0;
    steps=0;
    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()==solidEmpty){
            findRoute(grid,workingI,workingJ,ti,tj,steps,waste);
            if(steps>newsteps&&!(ti==1&&tj==1)){
                I=ti;
                J=tj;
                newsteps=steps;
                tileBlueButton.setI(I);
                tileBlueButton.setJ(J);
            }
        }
    }//found the dead end to place the Red Switch!
    grid[tileBlueButton.getI()][tileBlueButton.getJ()].setSolidity(solidReserved);

    findAdjacentEmptyTiles(grid,I,J,up,down,left,right);
    takenDirection=STOP;
    previousTakenDirection=STOP;
    if(up==0&&down==0&&left==0&&right==1){
        previousTakenDirection=takenDirection;
        takenDirection=RIGHT;
        tileZombieB.setI(I);
        tileZombieB.setJ(J+1);
    }
    else if(up==0&&down==0&&left==1&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=LEFT;
        tileZombieB.setI(I);
        tileZombieB.setJ(J-1);
    }
    else if(up==0&&down==1&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=DOWN;
        tileZombieB.setI(I+1);
        tileZombieB.setJ(J);
    }
    else if(up==1&&down==0&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=UP;
        tileZombieB.setI(I-1);
        tileZombieB.setJ(J);
    }
    grid[tileZombieB.getI()][tileZombieB.getJ()].setSolidity(solidReserved);


    deadEndCopy = deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileOrangeButton.setI(ti);
            tileOrangeButton.setJ(tj);
            break;
        }
    }
    grid[tileOrangeButton.getI()][tileOrangeButton.getJ()].setSolidity(solidReserved);

    findAdjacentEmptyTiles(grid,tileOrangeButton.getI(),tileOrangeButton.getJ(),up,down,left,right);
    takenDirection=STOP;
    previousTakenDirection=STOP;
    if(up==0&&down==0&&left==0&&right==1){
        previousTakenDirection=takenDirection;
        takenDirection=RIGHT;
    }
    else if(up==0&&down==0&&left==1&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=LEFT;
    }
    else if(up==0&&down==1&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=DOWN;
    }
    else if(up==1&&down==0&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=UP;
    }

    //trace to next junction
    stepsTilJunct=0;
    I=tileOrangeButton.getI();
    J=tileOrangeButton.getJ();

    walkToNextJunct(grid,I,J,workingI,workingJ,takenDirection,stepsTilJunct);
    findRoute(grid,workingI,workingJ,ti,tj,steps,waste);

    if(waste==UP){
        workingI--;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueDown);
    }
    else if(waste==DOWN){
        workingI++;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueUp);
    }
    else if(waste==LEFT){
        workingJ--;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueLeft);
    }
    else if(waste==RIGHT){
        workingJ++;
        tileBlueDoor.setI(workingI);
        tileBlueDoor.setJ(workingJ);
        tileBlueDoor.setAnim(animDoorBlueRight);
    }
    grid[tileBlueDoor.getI()][tileBlueDoor.getJ()].setSolidity(solidReserved);

    deadEndCopy = deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileZombieC.setI(ti);
            tileZombieC.setJ(tj);
            break;
        }
    }
    grid[tileZombieC.getI()][tileZombieC.getJ()].setSolidity(solidReserved);

    findAdjacentEmptyTiles(grid,tileZombieC.getI(),tileZombieC.getJ(),up,down,left,right);
    takenDirection=STOP;
    previousTakenDirection=STOP;
    if(up==0&&down==0&&left==0&&right==1){
        previousTakenDirection=takenDirection;
        takenDirection=RIGHT;
    }
    else if(up==0&&down==0&&left==1&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=LEFT;
    }
    else if(up==0&&down==1&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=DOWN;
    }
    else if(up==1&&down==0&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=UP;
    }

    //trace to next junction
    stepsTilJunct=0;
    I=tileZombieC.getI();
    J=tileZombieC.getJ();

    walkToNextJunct(grid,I,J,workingI,workingJ,takenDirection,stepsTilJunct);
    findRoute(grid,workingI,workingJ,ti,tj,steps,waste);

    if(waste==UP){
        workingI--;
        tileBlueDoorBeta.setI(workingI);
        tileBlueDoorBeta.setJ(workingJ);
        tileBlueDoorBeta.setAnim(animDoorBlueDown);
    }
    else if(waste==DOWN){
        workingI++;
        tileBlueDoorBeta.setI(workingI);
        tileBlueDoorBeta.setJ(workingJ);
        tileBlueDoorBeta.setAnim(animDoorBlueUp);
    }
    else if(waste==LEFT){
        workingJ--;
        tileBlueDoorBeta.setI(workingI);
        tileBlueDoorBeta.setJ(workingJ);
        tileBlueDoorBeta.setAnim(animDoorBlueLeft);
    }
    else if(waste==RIGHT){
        workingJ++;
        tileBlueDoorBeta.setI(workingI);
        tileBlueDoorBeta.setJ(workingJ);
        tileBlueDoorBeta.setAnim(animDoorBlueRight);
    }
    grid[tileBlueDoorBeta.getI()][tileBlueDoorBeta.getJ()].setSolidity(solidReserved);

    deadEndCopy = deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileZombieD.setI(ti);
            tileZombieD.setJ(tj);
            break;
        }
    }
    grid[tileZombieD.getI()][tileZombieD.getJ()].setSolidity(solidReserved);

    findAdjacentEmptyTiles(grid,tileZombieD.getI(),tileZombieD.getJ(),up,down,left,right);
    takenDirection=STOP;
    previousTakenDirection=STOP;
    if(up==0&&down==0&&left==0&&right==1){
        previousTakenDirection=takenDirection;
        takenDirection=RIGHT;
    }
    else if(up==0&&down==0&&left==1&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=LEFT;
    }
    else if(up==0&&down==1&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=DOWN;
    }
    else if(up==1&&down==0&&left==0&&right==0){
        previousTakenDirection=takenDirection;
        takenDirection=UP;
    }

    //trace to next junction
    stepsTilJunct=0;
    I=tileZombieD.getI();
    J=tileZombieD.getJ();

    walkToNextJunct(grid,I,J,workingI,workingJ,takenDirection,stepsTilJunct);
    findRoute(grid,workingI,workingJ,ti,tj,steps,waste);

    if(waste==UP){
        workingI--;
        tileOrangeDoorBeta.setI(workingI);
        tileOrangeDoorBeta.setJ(workingJ);
        tileOrangeDoorBeta.setAnim(animDoorOrangeDown);
    }
    else if(waste==DOWN){
        workingI++;
        tileOrangeDoorBeta.setI(workingI);
        tileOrangeDoorBeta.setJ(workingJ);
        tileOrangeDoorBeta.setAnim(animDoorOrangeUp);
    }
    else if(waste==LEFT){
        workingJ--;
        tileOrangeDoorBeta.setI(workingI);
        tileOrangeDoorBeta.setJ(workingJ);
        tileOrangeDoorBeta.setAnim(animDoorOrangeLeft);
    }
    else if(waste==RIGHT){
        workingJ++;
        tileOrangeDoorBeta.setI(workingI);
        tileOrangeDoorBeta.setJ(workingJ);
        tileOrangeDoorBeta.setAnim(animDoorOrangeRight);
    }
    grid[tileOrangeDoorBeta.getI()][tileOrangeDoorBeta.getJ()].setSolidity(solidReserved);

    deadEndCopy = deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileZombieE.setI(ti);
            tileZombieE.setJ(tj);
            break;
        }
    }
    grid[tileZombieE.getI()][tileZombieE.getJ()].setSolidity(solidReserved);

    deadEndCopy=deadEnd;
    while(deadEndCopy.size()!=0){
        tj=deadEndCopy.top();
        deadEndCopy.pop();
        ti=deadEndCopy.top();
        deadEndCopy.pop();

        if(grid[ti][tj].getSolidity()!=solidReserved){
            tileGold.setI(ti);
            tileGold.setJ(tj);
        }
    }
    grid[tileGold.getI()][tileGold.getJ()].setSolidity(solidReserved);
    grid[tileGold.getI()][tileGold.getJ()]=tileGold;
    grid[tileEnd.getI()][tileEnd.getJ()]=tileEnd;
    grid[tileZombieE.getI()][tileZombieE.getJ()]=tileZombieE;
    grid[tileZombieD.getI()][tileZombieD.getJ()]=tileZombieD;
    grid[tileZombieC.getI()][tileZombieC.getJ()]=tileZombieC;
    grid[tileZombieB.getI()][tileZombieB.getJ()]=tileZombieB;
    grid[tileZombieA.getI()][tileZombieA.getJ()]=tileZombieA;
    grid[tileOrangeButton.getI()][tileOrangeButton.getJ()]=tileOrangeButton;
    grid[tileBlueButton.getI()][tileBlueButton.getJ()]=tileBlueButton;
    grid[tileOrangeDoor.getI()][tileOrangeDoor.getJ()]=tileOrangeDoor;
    grid[tileOrangeDoorBeta.getI()][tileOrangeDoorBeta.getJ()]=tileOrangeDoorBeta;
    grid[tileBlueDoor.getI()][tileBlueDoor.getJ()]=tileBlueDoor;
    grid[tileBlueDoorBeta.getI()][tileBlueDoorBeta.getJ()]=tileBlueDoorBeta;
    grid[tileMazeMan.getI()][tileMazeMan.getJ()]=tileMazeMan;

    return 1;

}

int level(tile grid[][gridJ], int level){

    initLevel();
    setFont();
    iprintf("\x1b[1;1H-------Loading-------");
    if(level==1){
        showPicture(picOne);
        iprintf("\x1b[3;1Hhint:\n %s",HINTS[hintsUsed[0]]);
    }
    else if(level==2){
        showPicture(picTwo);
        iprintf("\x1b[3;1Hhint:\n %s",HINTS[hintsUsed[1]]);
    }
    else if(level==3){
        showPicture(picThree);
        iprintf("\x1b[3;1Hhint:\n %s",HINTS[hintsUsed[2]]);
    }
    else if(level==4){
        showPicture(picFour);
        iprintf("\x1b[3;1Hhint:\n %s",HINTS[hintsUsed[3]]);
    }
    else if(level==5){
        showPicture(picFive);
        iprintf("\x1b[3;1Hhint:\n %s",HINTS[hintsUsed[4]]);
    }

    while(1){

        makeMaze(grid,true);

        if(level==1){
            if(populateLevelOne(grid)==1){break;}
            else{secondDelay(1);  }
        }
        else if(level==2){
            if(populateLevelTwo(grid)==1){break;}
            else{secondDelay(1);  }
        }
        else if(level==3){
            if(populateLevelThree(grid)==1){break;}
            else{secondDelay(1);  }
        }
        else if(level==4){
            if(populateLevelFour(grid)==1){break;}
            else{secondDelay(1); }
        }
        else if(level==5){
            if(populateLevelFive(grid)==1){break;}
            else{secondDelay(1);  }
        }

    }

    iprintf("\x1b[21;1HPress 'A' to continue.");

    //ensures you're not holding down the A button
    while(1){
        scanKeys();
        int released = keysHeld();
        if(!(released & KEY_A)){

            break;
        }
    }
    secondDelay(0.1);

    while(1){
        scanKeys();
        int held = keysHeld();

        if(held & KEY_A){
            break;
		}

        secondDelay(0.05);
    }//end-while

    //these two stacks will be used to keep track of the open ends.
    stack<int> endStack;
    stack<int> nextEndStack;

    int collapseClock=0;
    int rockDelay=12;
    int rockFallSound=0;
    if(mode==NORMAL){rockDelay=9;}
    if(mode==HARD){rockDelay=6;}

    mmEffectEx(&keyPickupSound);
    secondDelay(0.5);

    loadPalette();
    loadIconsToMemory();
    setScreenForPlay();
    consoleClear();

    //initialise maze man stuff
    int mazeManDir=DOWN, previousDir=DOWN;
    int zombieADir=LEFT, zombieAPrevDir=LEFT;
    int zombieBDir=LEFT, zombieBPrevDir=LEFT;
    int zombieCDir=LEFT, zombieCPrevDir=LEFT;
    int zombieDDir=LEFT, zombieDPrevDir=LEFT;
    int zombieEDir=LEFT, zombieEPrevDir=LEFT;

    tileZombieB.incClock();
    tileZombieC.incClock();tileZombieC.incClock();
    tileZombieD.incClock();tileZombieD.incClock();tileZombieD.incClock();
    tileZombieE.incClock();tileZombieE.incClock();tileZombieE.incClock();tileZombieE.incClock();

    int currentI = 0,currentJ = 0;
    int zombiesCrushed=0;
    deathType=typeNone;
    goldAcquired=0;

    setFont();
    while(1){
        printScreen(grid);
        if(level==1){
            iprintf("\x1b[1;1HLevel 1");
            iprintf("\x1b[1;16HPoints: %7d",totalScore);
            iprintf("\x1b[3;1H'%s'",QUOTES[quotesUsed[0]]);
        }
        else if(level==2){
            iprintf("\x1b[1;1HLevel 2");
            iprintf("\x1b[1;16HPoints: %7d",totalScore);
            iprintf("\x1b[3;1H'%s'",QUOTES[quotesUsed[1]]);
        }
        else if(level==3){
            iprintf("\x1b[1;1HLevel 3");
            iprintf("\x1b[1;16HPoints: %7d",totalScore);
            iprintf("\x1b[3;1H'%s'",QUOTES[quotesUsed[2]]);
        }
        else if(level==4){
            iprintf("\x1b[1;1HLevel 4");
            iprintf("\x1b[1;16HPoints: %7d",totalScore);
            iprintf("\x1b[3;1H'%s'",QUOTES[quotesUsed[3]]);
        }
        else if(level==5){
            iprintf("\x1b[1;1HLevel 5");
            iprintf("\x1b[1;16HPoints: %7d",totalScore);
            iprintf("\x1b[3;1H'Time to Indiana Jones it'");
        }


        showZombieDead(grid,level);
        grid[tileGDoor.getI()][tileGDoor.getJ()]=tileGDoor;
        grid[tileGDoorBeta.getI()][tileGDoorBeta.getJ()]=tileGDoorBeta;
        grid[tileBlueDoor.getI()][tileBlueDoor.getJ()]=tileBlueDoor;
        grid[tileBlueDoorBeta.getI()][tileBlueDoorBeta.getJ()]=tileBlueDoorBeta;
        grid[tileOrangeDoor.getI()][tileOrangeDoor.getJ()]=tileOrangeDoor;
        grid[tileRDoor.getI()][tileRDoor.getJ()]=tileRDoor;
        grid[tileOrangeDoorBeta.getI()][tileOrangeDoorBeta.getJ()]=tileOrangeDoorBeta;

        showZombie(grid,level);

        if(collapseClock>120&&collapseClock%rockDelay==0&&level==5){
                if(grid[1][1].getID()!=tileRockID){
                    grid[1][1]=tileRock;
                    endStack.push(1);
                    endStack.push(1);
                }
                else{

                    //flood mechanic!
                    int tempI=0,tempJ=0;
                    while(!endStack.empty()){
                        tempJ=endStack.top();
                        endStack.pop();
                        tempI=endStack.top();
                        endStack.pop();

                        //crush mazeman
                        //look right
                        if(grid[tempI][tempJ+1].getID()==tileMazeManID){
                            deathBool=true;
                            nextEndStack.push(tempI);
                            nextEndStack.push(tempJ+1);
                        }
                        //look left
                        if(grid[tempI][tempJ-1].getID()==tileMazeManID){
                            deathBool=true;
                            nextEndStack.push(tempI);
                            nextEndStack.push(tempJ-1);
                        }
                        //look up
                        if(grid[tempI-1][tempJ].getID()==tileMazeManID){
                            deathBool=true;
                            nextEndStack.push(tempI-1);
                            nextEndStack.push(tempJ);
                        }
                        //look down
                        if(grid[tempI+1][tempJ].getID()==tileMazeManID){
                            deathBool=true;
                            nextEndStack.push(tempI+1);
                            nextEndStack.push(tempJ);
                        }

                        //crush zombiea
                        //look right
                        if(grid[tempI][tempJ+1].getID()==tileZombieAID){
                            zombiesCrushed++;
                            tileZombieA.setHealth(0);
                            grid[tempI][tempJ+1]=tileEmpty;
                        }
                        //look left
                        if(grid[tempI][tempJ-1].getID()==tileZombieAID){
                            zombiesCrushed++;
                            tileZombieA.setHealth(0);
                            grid[tempI][tempJ-1]=tileEmpty;
                        }
                        //look up
                        if(grid[tempI-1][tempJ].getID()==tileZombieAID){
                            zombiesCrushed++;
                            tileZombieA.setHealth(0);
                            grid[tempI-1][tempJ]=tileEmpty;
                        }
                        //look down
                        if(grid[tempI+1][tempJ].getID()==tileZombieAID){
                            zombiesCrushed++;
                            tileZombieA.setHealth(0);
                            grid[tempI+1][tempJ]=tileEmpty;
                        }

                        //crush zombiea
                        //look right
                        if(grid[tempI][tempJ+1].getID()==tileZombieBID){
                            zombiesCrushed++;
                            tileZombieB.setHealth(0);
                            grid[tempI][tempJ+1]=tileEmpty;
                        }
                        //look left
                        if(grid[tempI][tempJ-1].getID()==tileZombieBID){
                            zombiesCrushed++;
                            tileZombieB.setHealth(0);
                            grid[tempI][tempJ-1]=tileEmpty;
                        }
                        //look up
                        if(grid[tempI-1][tempJ].getID()==tileZombieBID){
                            zombiesCrushed++;
                            tileZombieB.setHealth(0);
                            grid[tempI-1][tempJ]=tileEmpty;
                        }
                        //look down
                        if(grid[tempI+1][tempJ].getID()==tileZombieBID){
                            zombiesCrushed++;
                            tileZombieB.setHealth(0);
                            grid[tempI+1][tempJ]=tileEmpty;
                        }


                        //cave in
                        //look right
                        if(grid[tempI][tempJ+1].getID()==tileEmptyID){
                            nextEndStack.push(tempI);
                            nextEndStack.push(tempJ+1);
                        }
                        //look left
                        if(grid[tempI][tempJ-1].getID()==tileEmptyID){
                            nextEndStack.push(tempI);
                            nextEndStack.push(tempJ-1);
                        }
                        //look up
                        if(grid[tempI-1][tempJ].getID()==tileEmptyID){
                            nextEndStack.push(tempI-1);
                            nextEndStack.push(tempJ);
                        }
                        //look down
                        if(grid[tempI+1][tempJ].getID()==tileEmptyID){
                            nextEndStack.push(tempI+1);
                            nextEndStack.push(tempJ);
                        }

                    }

                    //swap the stacks around, empty the temp stack.
                    endStack = nextEndStack;
                    while(!nextEndStack.empty()){
                        int workingJ=nextEndStack.top();
                        nextEndStack.pop();
                        int workingI=nextEndStack.top();
                        nextEndStack.pop();
                        grid[workingI][workingJ]=tileRock;
                    }
                    rockFallSound++;
                    if(rockFallSound%4==0){mmEffectEx(&rockFall);}

                }

                if(deathBool==true){
                    mmEffectEx(&doorStuck);
                    deathType=typeRock;
                    break;
                }
        }//end rock collapse

        if(victBool==false){grid[tileMazeMan.getI()][tileMazeMan.getJ()]=tileMazeMan;}

        if(victBool==true){
            grid[tileMazeMan.getI()][tileMazeMan.getJ()]=tileEmpty;
            break;
        }

        if(tileMazeMan.getHealth()<=0){
            deathType=typeZomb;
            break;
        }

        //removes the effects from any attacks
        for(int i=0;i<gridI;i++){
            for(int j=0;j<gridJ;j++){
                if (grid[i][j].getID()==tileEmptyID){
                    grid[i][j].setSolidity(solidEmpty);
                }
            }
        }//end -attack reset

        currentI = tileMazeMan.getI();
        currentJ = tileMazeMan.getJ();

        //#############################################
        //#######         mazeman control etc   #######
        //#############################################

        scanKeys();
        int held = keysHeld();
        getUserInput(grid,mazeManDir,previousDir,held);

        //1,2,3,4,5,6,10,12,15,20,30,60
        if(tileMazeMan.getClock()%tileMazeMan.getDelay()==0&&victBool==false){

            //CHECK FOR BUTTON PUSH
            if(checkActions(grid,currentI,currentJ,mazeManDir,previousDir,held)==-1){return -1;}

            //SETS THE ATTACK SPACE, (if you're attacking)
            mazeManAttack(grid,mazeManDir,previousDir);

            //DEAL WITH MAZEMAN MOVEMENTS
            if(mazeManDir!=STOP){
                actorMove(grid,tileMazeMan,mazeManDir);
            }
            else{
                tileMazeMan.defaultSprite();
                grid[currentI][currentJ]=tileMazeMan;
            }

        }//END Maze Man Stuff


        if(tileZombieA.getClock()%tileZombieA.getDelay()==0&&tileZombieA.getHealth()!=0){
            zombieMovement(grid,tileZombieA,zombieADir,zombieAPrevDir,zombieMoan,mazeManDir);
        }
        if(tileZombieB.getClock()%tileZombieB.getDelay()==0&&tileZombieB.getHealth()!=0){
            zombieMovement(grid,tileZombieB,zombieBDir,zombieBPrevDir,zombieMoan,mazeManDir);
        }
        if(tileZombieC.getClock()%tileZombieC.getDelay()==0&&tileZombieC.getHealth()!=0){
            zombieMovement(grid,tileZombieC,zombieCDir,zombieCPrevDir,zombieMoan,mazeManDir);
        }
        if(tileZombieD.getClock()%tileZombieD.getDelay()==0&&tileZombieD.getHealth()!=0){
            zombieMovement(grid,tileZombieD,zombieDDir,zombieDPrevDir,zombieMoan,mazeManDir);
        }
        if(tileZombieE.getClock()%tileZombieE.getDelay()==0&&tileZombieE.getHealth()!=0){
            zombieMovement(grid,tileZombieE,zombieEDir,zombieEPrevDir,zombieMoan,mazeManDir);
        }

        frameCounter++;
        if(tileMazeMan.getSteps()>0){collapseClock++;}
        tileMazeMan.incClock();
        tileZombieA.incClock();
        tileZombieB.incClock();
        tileZombieC.incClock();
        tileZombieD.incClock();
        tileZombieE.incClock();
        swiWaitForVBlank();

    }

    if(victBool==true){
        //calculates score
        //allows a bonus 500 points for completing it in minimal amount of steps
        int killBonus=0;
        int killAmount=0;

        //Sets up the points for each zombie kill!
        if(mode==EASY){killAmount=100;}
        else if(mode==NORMAL){killAmount=200;}
        else if(mode==HARD){killAmount=300;}

        if(tileZombieA.getHealth()==0&&tileZombieA.getI()!=-1){
            killBonus+=killAmount;
        }
        if(tileZombieB.getHealth()==0&&tileZombieB.getI()!=-1){
            killBonus+=killAmount;
        }
        if(tileZombieC.getHealth()==0&&tileZombieC.getI()!=-1){
            killBonus+=killAmount;
        }
        if(tileZombieD.getHealth()==0&&tileZombieD.getI()!=-1){
            killBonus+=killAmount;
        }
        if(tileZombieE.getHealth()==0&&tileZombieE.getI()!=-1){
            killBonus+=killAmount;
        }

        //correction for crushed zombies, you dont deserve points for it!
        killBonus-=(zombiesCrushed*killAmount);
        if(killBonus<0){killBonus=0;}

        //sets up the points for this difficulty of level
        int diffBonus=300;
        if(mode==NORMAL){diffBonus=400;}
        else if(mode==HARD){diffBonus=500;}

        int goldscore=0;
        if (goldAcquired==1){goldscore=200;}

        //hearts bonus
        int heartsBonus=90;
        int totalHearts = tileMazeMan.getTotalHealth();

        if(totalHearts==9){heartsBonus=tileMazeMan.getHealth()*10;}
        else if(totalHearts==6){heartsBonus=tileMazeMan.getHealth()*20;}
        else if(totalHearts==3){heartsBonus=tileMazeMan.getHealth()*50;}

        int totalScoreThisLevel=killBonus+diffBonus+goldscore+heartsBonus;

        //bonus points calculated, time to print the score!
        iprintf("\x1b[5;1HSCORE");
        iprintf("\x1b[6;1H-----------------------------");
        secondDelay(1);
        iprintf("\x1b[7;1HLevel Complete:          %4d", diffBonus);
        mmEffectEx(&keyPickupSound);
        secondDelay(1);
        iprintf("\x1b[8;1HKill Bonus:              %4d", killBonus);
        mmEffectEx(&keyPickupSound);
        secondDelay(1);
        iprintf("\x1b[9;1HGold Bonus:              %4d", goldscore);
        mmEffectEx(&keyPickupSound);
        secondDelay(1);
        iprintf("\x1b[10;1HHearts Bonus:            %4d", heartsBonus);
        mmEffectEx(&keyPickupSound);
        secondDelay(1);
        iprintf("\x1b[11;1H-----------------------------");
        iprintf("\x1b[12;1HTotal:                   %4d",totalScoreThisLevel);

        totalScore+=totalScoreThisLevel;
        iprintf("\x1b[1;16HPoints: %7d",totalScore);
        waitAtoContinue();
        return 1;
    }
    else{
        return deathOccured();
    }

}//level completed

void aboutScreen(){
    setFont();
    iprintf("\x1b[1;1HAbout MazeMan");
    iprintf("\x1b[3;1H---------------------------");
    iprintf("\x1b[5;1HBuilt using LibNDS");
    iprintf("\x1b[7;1Hcreated by Luke Rodgers");
    iprintf("\x1b[8;1Hwww.lukerodgers.co.uk");
    iprintf("\x1b[10;1Hsound effects from FreeSfx");
    iprintf("\x1b[12;1Ha special thanks to,");
    iprintf("\x1b[13;1Hwww.samforson.co.uk");
    iprintf("\x1b[14;1Hand everyone who helped test.");

    waitBtoReturn();
}

void howToPlay(){
    showPicture(picInst);
    iprintf("\x1b[1;1HHow to Play");
    iprintf("\x1b[3;1H------------------------------");
    iprintf("\x1b[5;1HUse the d-pad to move");
    iprintf("\x1b[7;1HUse A to operate switches");
    iprintf("\x1b[9;1HUse B to swipe the sword");
    iprintf("\x1b[1;23H^Health^");

    waitBtoReturn();
    showPicture(picMenu);
}

int playGame(tile grid[][gridJ]){

    mmEffectEx(&ladderSound);
    int position=1;
    showPicture(picInst);
    //ensures you're not holding down the A button
    while(1){
        scanKeys();
        int released = keysHeld();
        if(!(released & KEY_A)){
            break;
        }
    }
    secondDelay(0.1);
    while(1){

        scanKeys();
        int held = keysHeld();

        if(held & KEY_A){
            if(position==0){
                mode=EASY;
                break;
            }
            else if(position==1){
                mode=NORMAL;
                break;
            }
            else if(position==2){
                mode=HARD;
                break;
            }
        }

        if(held & KEY_B){
            showPicture(picMenu);
            mmEffectEx(&doorStuck);
            return -1;
        }

        if(held & KEY_UP){          if(position!=0){position--; mmEffectEx(&ladderSound);}}
        else if(held & KEY_DOWN){   if(position!=2){position++; mmEffectEx(&ladderSound);}}

        iprintf("\x1b[3;5HSelect Difficulty");
        iprintf("\x1b[5;5H-----------------");
        iprintf("\x1b[7;5HEasy");
        iprintf("\x1b[9;5HNormal");
        iprintf("\x1b[11;5HHard");

        if(position==0){
            iprintf("\x1b[7;3H>");
            iprintf("\x1b[9;3H ");
            iprintf("\x1b[11;3H ");
        }
        else if(position==1){
            iprintf("\x1b[7;3H ");
            iprintf("\x1b[9;3H>");
            iprintf("\x1b[11;3H ");
        }
        else if(position==2){
            iprintf("\x1b[7;3H ");
            iprintf("\x1b[9;3H ");
            iprintf("\x1b[11;3H>");
        }

        secondDelay(0.1);
        frameCounter++;

    }

    //mode has been selected
    //prepare the game!
    tileMazeMan.make(tileMazeManID,solidPlayer,animManDown);

    //GET THE QUOTES FOR THIS PLAYTHROUGH
    int *numbers=(int *)malloc(QUOTECOUNT*sizeof(int));
    int x=0;

    for(int i=0;i<QUOTECOUNT;i++){
        numbers[i]=0;
    }

    for(int i=0;i<4;i++){
        while(1){
            x=rand()%QUOTECOUNT;
            if(numbers[x]==0){
                quotesUsed[i]=x;
                numbers[x]=1;
                break;
            }
        }
    }
    free(numbers);
    //QUOTES ACQUIRED

    //GET THE HINTS FOR THIS PLAYTHROUGH
    numbers=(int *)malloc(HINTCOUNT*sizeof(int));

    for(int i=0;i<HINTCOUNT;i++){
        numbers[i]=0;
    }

    for(int i=0;i<5;i++){
        while(1){
            x=rand()%HINTCOUNT;

            if(numbers[x]==0){
                hintsUsed[i]=x;
                numbers[x]=1;
                break;
            }
        }
    }
    //HINTS ACQUIRED

    free(numbers);
    totalScore=0;
    for(int i=1;i<6;i++){
        while(1){
            int status = level(grid,i);
            //quit
            if (status==-1){
                return -1;
            }
            //die
            if (status==0){
                if(mode==EASY){totalScore-=tryEasy;}
                else if(mode==NORMAL){totalScore-=tryNormal;}
                else if(mode==HARD){totalScore-=tryHard;}
            }
            //victory

            if (status==1){
                break;
            }
        }
    }//end for game playthrough

    return 1;

}

//factors of 60: 1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60
int main(void) {

    powerOn(POWER_ALL_2D);

    vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000,
                     VRAM_B_MAIN_BG_0x06020000,
                     VRAM_C_SUB_BG_0x06200000,
                     VRAM_D_LCD);

	mmInitDefaultMem((mm_addr)soundbank_bin);
    initSounds();

    showPicture(picMenu);

    tile grid[gridI][gridJ];

    for(int i=0;i<gridI;i++){
        for(int j=0;j<gridJ;j++){
            grid[i][j]=tileEmpty;
        }
    }//initialise

    setFont();
    int highScoreFlag=0;
    int position=0;
    totalScore=0;

    while(1){

        scanKeys();
        int held = keysHeld();
        srand(frameCounter);
        if(held & KEY_A){
            if(position==0){
                highScoreFlag=0;
                totalScore=0;
                consoleClear();
                int game=playGame(grid);
                if(mode==EASY){
                    if(totalScore>highscoreEasy&&game!=-1){highscoreEasy=totalScore;highScoreFlag=1;}
                    else{highscoreEasy=highscoreEasy;}
                }
                else if(mode==NORMAL){
                    if(totalScore>highscoreNormal&&game!=-1){highscoreNormal=totalScore;highScoreFlag=2;}
                    else{highscoreNormal=highscoreNormal;}
                }
                else if(mode==HARD){
                    if(totalScore>highscoreHard&&game!=-1){highscoreHard=totalScore;highScoreFlag=3;}
                    else{highscoreHard=highscoreHard;}
                }

                consoleClear();
                showPicture(picMenu);
            }
            else if(position==1){
                consoleClear();
                howToPlay();
                consoleClear();
            }
            else if(position==2){
                consoleClear();
                aboutScreen();
                consoleClear();
            }
		}

        if(held & KEY_UP){ if(position!=0){position--; mmEffectEx(&ladderSound);}}
        else if(held & KEY_DOWN){ if(position!=2){position++; mmEffectEx(&ladderSound);}}

        iprintf("\x1b[1;5HMain Menu");
        iprintf("\x1b[3;5H---------");
        iprintf("\x1b[5;5HStart");
        iprintf("\x1b[7;5HHow To Play");
        iprintf("\x1b[9;5HAbout");

        if(position==0){
            iprintf("\x1b[5;3H>");
            iprintf("\x1b[7;3H ");
            iprintf("\x1b[9;3H ");
        }
        else if(position==1){
            iprintf("\x1b[5;3H ");
            iprintf("\x1b[7;3H>");
            iprintf("\x1b[9;3H ");
        }
        else if(position==2){
            iprintf("\x1b[5;3H ");
            iprintf("\x1b[7;3H ");
            iprintf("\x1b[9;3H>");
        }

        if(highScoreFlag==1){iprintf("\x1b[17;18H*New Score!*");}
        else if(highScoreFlag==2){iprintf("\x1b[19;18H*New Score!*");}
        else if(highScoreFlag==3){iprintf("\x1b[21;18H*New Score!*");}

        iprintf("\x1b[13;5HHigh Scores");
        iprintf("\x1b[15;5H------------");
        iprintf("\x1b[17;5HEasy:   %4d",highscoreEasy);
        iprintf("\x1b[19;5HNormal: %4d",highscoreNormal);
        iprintf("\x1b[21;5HHard:   %4d",highscoreHard);

        secondDelay(0.1);
        frameCounter++;

    }

	return 0;
}
