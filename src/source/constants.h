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

int animManLeft [4]     = {spriteManLeft,spriteManLeftA,spriteManLeftB,-1};
int animManRight [4]    = {spriteManDown,spriteManDownA,spriteManDownB,-1};
int animManUp [4]       = {spriteManUp,spriteManUpA,spriteManUpB,-1};
int animManDown [4]     = {spriteManDown, spriteManDownA, spriteManDownB,-1};

//items
int animDoorUp [5]    = {spriteDoorDown,spriteDoorDownB,spriteDoorDownC,spriteDoorDownD,-1};
int animDoorDown [5]    = {spriteDoorDown,spriteDoorDownB,spriteDoorDownC,spriteDoorDownD,-1};
int animDoorLeft [5]    = {spriteDoorLeft,spriteDoorLeftB,spriteDoorLeftC,spriteDoorLeftD,-1};
int animDoorRight [5]    = {spriteDoorRight,spriteDoorRightB,spriteDoorRightC,spriteDoorRightD,-1};

int animDoorUpB [5]    = {spriteDoorDownD,spriteDoorDownC,spriteDoorDownB,spriteDoorDown,-1};
int animDoorDownB [5]    = {spriteDoorDownD,spriteDoorDownC,spriteDoorDownB,spriteDoorDown,-1};
int animDoorLeftB [5]    = {spriteDoorLeftD,spriteDoorLeftC,spriteDoorLeftB,spriteDoorLeft,-1};
int animDoorRightB [5]    = {spriteDoorRightD,spriteDoorRightC,spriteDoorRightB,spriteDoorRight,-1};

int animRButton [4]     = {spriteRButtonUp, spriteRButtonMiddle, spriteRButtonDown,-1};
int animRButtonB [4]     = {spriteRButtonDown, spriteRButtonMiddle, spriteRButtonUp,-1};

int animDoorGUp [5] = {spriteDoorGDown,spriteDoorGDownB, spriteDoorGDownC, spriteDoorGDownD, -1};
int animDoorGDown [5] = {spriteDoorGDown,spriteDoorGDownB, spriteDoorGDownC, spriteDoorGDownD, -1};
int animDoorGRight [5] = {spriteDoorGRight,spriteDoorGRightB,spriteDoorGRightC,spriteDoorGRightD, -1};
int animDoorGLeft [5] = {spriteDoorGLeft,spriteDoorGLeftB,spriteDoorGLeftC,spriteDoorGLeftD,-1};

int animDoorGUpB [5] = {spriteDoorGDownD,spriteDoorGDownC, spriteDoorGDownB, spriteDoorGDown, -1};
int animDoorGDownB [5] = {spriteDoorGDownD,spriteDoorGDownC, spriteDoorGDownB, spriteDoorGDown, -1};
int animDoorGRightB [5] = {spriteDoorGRightD,spriteDoorGRightC,spriteDoorGRightB,spriteDoorGRight, -1};
int animDoorGLeftB [5] = {spriteDoorGLeftD,spriteDoorGLeftC,spriteDoorGLeftB,spriteDoorGLeft,-1};

int animGButton [4]     = {spriteGButtonUp, spriteGButtonMiddle, spriteGButtonDown,-1};
int animGButtonB [4]     = {spriteGButtonDown, spriteGButtonMiddle, spriteGButtonUp,-1};

int animEnd [6]         = {spriteEndDown,spriteEndDownB, spriteEndDownC,spriteEndDownD,spriteEndDownE,-1};

int animZombieUp [4] = {spriteZombieUp,spriteZombieUpB,spriteZombieUpC,-1};
int animZombieLeft [4] = {spriteZombieLeft,spriteZombieLeftB,spriteZombieLeftC,-1};
int animZombieRight [4] = {spriteZombieRight,spriteZombieRightB,spriteZombieRightC,-1};
int animZombieDown [4] = {spriteZombieRight,spriteZombieRightB,spriteZombieRightC,-1};

int animZombieBLeft [4] = {spriteZombieBLeft,spriteZombieBLeftB,spriteZombieBLeftC,-1};
int animZombieBUp [4] = {spriteZombieBUp,spriteZombieBUpB,spriteZombieBUpC,-1};
int animZombieBRight [4] = {spriteZombieBRight,spriteZombieBRightB,spriteZombieBRightC,-1};
int animZombieBDown [4] = {spriteZombieBRight,spriteZombieBRightB,spriteZombieBRightC,-1};

int animZombieCLeft [4] = {spriteZombieCLeft,spriteZombieCLeftB,spriteZombieCLeftC,-1};
int animZombieCUp [4] = {spriteZombieCUp,spriteZombieCUpB,spriteZombieCUpC,-1};
int animZombieCRight [4] = {spriteZombieCRight,spriteZombieCRightB,spriteZombieCRightC,-1};
int animZombieCDown [4] = {spriteZombieCRight,spriteZombieCRightB,spriteZombieCRightC,-1};

int animBlueButton [4]     = {spriteBlueButtonUp, spriteBlueButtonMiddle, spriteBlueButtonDown,-1};
int animBlueButtonB [4]     = {spriteBlueButtonDown, spriteBlueButtonMiddle, spriteBlueButtonUp,-1};

int animDoorBlueUp [5] = {spriteDoorBlueDown,spriteDoorBlueDownB, spriteDoorBlueDownC, spriteDoorBlueDownD, -1};
int animDoorBlueDown [5] = {spriteDoorBlueDown,spriteDoorBlueDownB, spriteDoorBlueDownC, spriteDoorBlueDownD, -1};
int animDoorBlueLeft [5] = {spriteDoorBlueLeft,spriteDoorBlueLeftB,spriteDoorBlueLeftC,spriteDoorBlueLeftD, -1};
int animDoorBlueRight [5] = {spriteDoorBlueRight,spriteDoorBlueRightB,spriteDoorBlueRightC,spriteDoorBlueRightD,-1};

int animDoorBlueUpB [5] = {spriteDoorBlueDownD,spriteDoorBlueDownC, spriteDoorBlueDownB, spriteDoorBlueDown, -1};
int animDoorBlueDownB [5] = {spriteDoorBlueDownD,spriteDoorBlueDownC, spriteDoorBlueDownB, spriteDoorBlueDown, -1};
int animDoorBlueLeftB [5] = {spriteDoorBlueLeftD,spriteDoorBlueLeftC,spriteDoorBlueLeftB,spriteDoorBlueLeft, -1};
int animDoorBlueRightB [5] = {spriteDoorBlueRightD,spriteDoorBlueRightC,spriteDoorBlueRightB,spriteDoorBlueRight,-1};

int animDoorOrangeUp [5] = {spriteDoorOrangeDown,spriteDoorOrangeDownB, spriteDoorOrangeDownC, spriteDoorOrangeDownD, -1};
int animDoorOrangeDown [5] = {spriteDoorOrangeDown,spriteDoorOrangeDownB, spriteDoorOrangeDownC, spriteDoorOrangeDownD, -1};
int animDoorOrangeLeft [5] = {spriteDoorOrangeLeft,spriteDoorOrangeLeftB,spriteDoorOrangeLeftC,spriteDoorOrangeLeftD, -1};
int animDoorOrangeRight [5] = {spriteDoorOrangeRight,spriteDoorOrangeRightB,spriteDoorOrangeRightC,spriteDoorOrangeRightD,-1};

int animDoorOrangeUpB [5] = {spriteDoorOrangeDownD,spriteDoorOrangeDownC, spriteDoorOrangeDownB, spriteDoorOrangeDown, -1};
int animDoorOrangeDownB [5] = {spriteDoorOrangeDownD,spriteDoorOrangeDownC, spriteDoorOrangeDownB, spriteDoorOrangeDown, -1};
int animDoorOrangeLeftB [5] = {spriteDoorOrangeLeftD,spriteDoorOrangeLeftC,spriteDoorOrangeLeftB,spriteDoorOrangeLeft, -1};
int animDoorOrangeRightB [5] = {spriteDoorOrangeRightD,spriteDoorOrangeRightC,spriteDoorOrangeRightB,spriteDoorOrangeRight,-1};

int animOrangeButton [4]     = {spriteOrangeButtonUp, spriteOrangeButtonMiddle, spriteOrangeButtonDown,-1};
int animOrangeButtonB [4]     = {spriteOrangeButtonDown, spriteOrangeButtonMiddle, spriteOrangeButtonUp,-1};

int animGold [2]     = {spriteGold,-1};

const int UP = 0, LEFT = 2, DOWN = 1, RIGHT = 3, STOP=4;
const int EASY=0, NORMAL=1, HARD=2;
const int picMenu=0, picOne=1, picTwo=3, picThree=4, picFour=2, picFive=5, picInst=6, picDeath=7;
const int typeRock=0, typeZomb=1, typeNone=2;
