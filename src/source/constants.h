#ifndef CONSTANTS_H
#define CONSTANTS_H

#define QUOTECOUNT 9
#define HINTCOUNT 8

extern const char *QUOTES[QUOTECOUNT];
extern const char *HINTS[HINTCOUNT];

const int tileEndID=0, tileMazeManID=1, tileWallID=2, tileEmptyID=3, tileRDoorID=6, tileRedButtonID=7,
            tileGDoorID=9, tileGreenButtonID=10, tileZombieAID=11, tileZombieCID=12, tileRockID=14,
            tileZombieBID=15,tileBlueButtonID=16, tileBlueDoorID=17, tileBlueButtonBetaID=18, tileBlueDoorBetaID=19, tileRDoorBetaID=20,
            tileRedButtonBetaID=21,tileGDoorBetaID=22, tileGreenButtonBetaID=23, tileOrangeButtonID=24, tileOrangeButtonBetaID=25,
            tileOrangeDoorID=26, tileOrangeDoorBetaID=27, tileZombieDID=28, tileZombieEID=29, tileGoldID=35;
const int solidEmpty=0, solidSolid=1, solidItem=2, solidPlayer=3, solidEnemy=4, solidAttack=5, solidReserved=6;

const int spriteEmpty=0, spriteRButtonUp=1, spriteRButtonDown=2, spriteGButtonUp=3, spriteDoorDown=4, spriteWall=5, spriteEndDown=6,
          spriteManDownA=7, spriteManUpA=8, spriteManLeft=9, spriteGButtonMiddle=10, spriteDoorLeft=11, spriteDoorRight=12;

const int spriteWallU=13, spriteWallD=14, spriteWallL=15, spriteWallR=16, spriteWallUD=17, spriteWallUL=18, spriteWallUR=19,
            spriteWallDL=20, spriteWallDR=21, spriteWallLR=22, spriteWallUDLR=23, spriteWallULD=24, spriteWallULR=25, spriteWallURD=26,
            spriteWallDLR=27,spriteWallSolo=28,spriteRButtonMiddle=29, spriteDoorLeftB=30, spriteDoorLeftC=31, spriteManDownB=32, spriteManUpB=33,
            spriteManDown=34, spriteManUp=35,spriteManLeftA=36,spriteManLeftB=37, spriteGButtonDown=38, spriteRock=39,
            spriteDoorDownB=40, spriteDoorDownC=41, spriteDoorDownD=42, spriteDoorLeftD=43, spriteDoorRightB=44, spriteDoorRightC=45,
            spriteDoorRightD=46, spriteEndDownB=47, spriteEndDownC=48, spriteEndDownD=49, spriteEndDownE=50,
            spriteDoorGDown=60, spriteDoorGDownB=61, spriteDoorGDownC=62, spriteDoorGDownD=63,
            spriteDoorGLeft=64, spriteDoorGLeftB=65, spriteDoorGLeftC=66, spriteDoorGLeftD=67,
            spriteDoorGRight=68, spriteDoorGRightB=69, spriteDoorGRightC=70, spriteDoorGRightD=71,
            spriteBlueButtonUp=72, spriteBlueButtonMiddle=73, spriteBlueButtonDown=74,
            spriteManAttLeft=76, spriteManAttRight=77, spriteManAttUp=78, spriteManAttDown=79,
            spriteZombieUp=80, spriteZombieUpB=81, spriteZombieUpC=82, spriteZombieLeft=83, spriteZombieLeftB=84, spriteZombieLeftC=85,
            spriteZombieRight=86, spriteZombieRightB=87, spriteZombieRightC=88, spriteZombieBLeft=89, spriteZombieBLeftB=90, spriteZombieBLeftC=91,
            spriteZombieBUp=92, spriteZombieBUpB=93, spriteZombieBUpC=94, spriteZombieBRight=95, spriteZombieBRightB=96, spriteZombieBRightC=97,
            spriteZombieCUp=98, spriteZombieCUpB=99, spriteZombieCUpC=100, spriteZombieCLeft=101, spriteZombieCLeftB=102, spriteZombieCLeftC=103,
            spriteZombieCRight=104, spriteZombieCRightB=105, spriteZombieCRightC=106, spriteZombieDead=107,
            spriteHeartFull=108, spriteHeartEmpty=109,
            spriteDoorBlueDown=110, spriteDoorBlueDownB=111, spriteDoorBlueDownC=112, spriteDoorBlueDownD=113,
            spriteDoorBlueLeft=114, spriteDoorBlueLeftB=115, spriteDoorBlueLeftC=116, spriteDoorBlueLeftD=117,
            spriteDoorBlueRight=118, spriteDoorBlueRightB=119, spriteDoorBlueRightC=120, spriteDoorBlueRightD=121,
            spriteDoorOrangeDown=122, spriteDoorOrangeDownB=123, spriteDoorOrangeDownC=124, spriteDoorOrangeDownD=125,
            spriteDoorOrangeLeft=126, spriteDoorOrangeLeftB=127, spriteDoorOrangeLeftC=128, spriteDoorOrangeLeftD=129,
            spriteDoorOrangeRight=130, spriteDoorOrangeRightB=131, spriteDoorOrangeRightC=132, spriteDoorOrangeRightD=133,
            spriteOrangeButtonUp=134, spriteOrangeButtonMiddle=135, spriteOrangeButtonDown=136,spriteGold=141;

extern int animManLeft [4];
extern int animManRight [4];
extern int animManUp [4];
extern int animManDown [4];

//items
extern int animDoorUp [5];
extern int animDoorDown [5];
extern int animDoorLeft [5];
extern int animDoorRight [5];

extern int animDoorUpB [5];
extern int animDoorDownB [5];
extern int animDoorLeftB [5];
extern int animDoorRightB [5];

extern int animRButton [4];
extern int animRButtonB [4];

extern int animDoorGUp [5];
extern int animDoorGDown [5];
extern int animDoorGRight [5];
extern int animDoorGLeft [5];

extern int animDoorGUpB [5];
extern int animDoorGDownB [5];
extern int animDoorGRightB [5];
extern int animDoorGLeftB [5];

extern int animGButton [4];
extern int animGButtonB [4];

extern int animEnd [6];

extern int animZombieUp [4];
extern int animZombieLeft [4];
extern int animZombieRight [4];
extern int animZombieDown [4];

extern int animZombieBLeft [4];
extern int animZombieBUp [4];
extern int animZombieBRight [4];
extern int animZombieBDown [4];

extern int animZombieCLeft [4];
extern int animZombieCUp [4];
extern int animZombieCRight [4];
extern int animZombieCDown [4];

extern int animBlueButton [4];
extern int animBlueButtonB [4];

extern int animDoorBlueUp [5];
extern int animDoorBlueDown [5];
extern int animDoorBlueLeft [5];
extern int animDoorBlueRight [5];

extern int animDoorBlueUpB [5];
extern int animDoorBlueDownB [5];
extern int animDoorBlueLeftB [5];
extern int animDoorBlueRightB [5];

extern int animDoorOrangeUp [5];
extern int animDoorOrangeDown [5];
extern int animDoorOrangeLeft [5];
extern int animDoorOrangeRight [5];

extern int animDoorOrangeUpB [5];
extern int animDoorOrangeDownB [5];
extern int animDoorOrangeLeftB [5];
extern int animDoorOrangeRightB [5];

extern int animOrangeButton [4];
extern int animOrangeButtonB [4];

extern int animGold [2];

const int UP = 0, LEFT = 2, DOWN = 1, RIGHT = 3, STOP=4;
const int EASY=0, NORMAL=1, HARD=2;
const int picMenu=0, picOne=1, picTwo=3, picThree=4, picFour=2, picFive=5, picInst=6, picDeath=7;
const int typeRock=0, typeZomb=1, typeNone=2;

const int gridJ=31; const int gridI=23;

#endif