#ifndef LEVEL_H
#define LEVEL_H

#include "constants.h"
#include "player.h"
#include "sound.h"
#include "item.h"


extern bool victBool, deathBool;
extern int frameCounter;
extern int totalScore;
extern int mode;

extern int goldAcquired;
extern int deathType;
extern int highscoreEasy;
extern int highscoreNormal;
extern int highscoreHard;
extern int tryEasy;
extern int tryNormal;
extern int tryHard;

extern int quotesUsed[4];
extern int hintsUsed[5];

extern void initTiles();

//initialise level variables and all
extern void initLevel();

extern void setFont(void);

extern void showPicture(int pic);

extern void secondDelay(double I);

extern int deathOccured();

extern void setScreenForPlay();

extern int findRoute(tile grid[][gridJ],int currentI, int currentJ, int desiredI, int desiredJ,
    int &stepsRequired, int &direction);

extern void findAdjacentEmptyTiles(tile grid[][gridJ], int I, int J, int &up, int &down, int &left, int &right);

//returns the location of the tile which would allow you to make a choice, a new direction
extern void walkToNextJunct(tile grid[][gridJ], int I, int J, int &junctI, int &junctJ, int &direction, int &steps);

//random movement for characters such as zombies
extern void randomMovement(tile grid[][gridJ],actor &T, int &direction,int &previousDir, int &steps);

extern void zombieMovement(tile grid[][gridJ],actor &T, int &dir, int &prevDir, mm_sound_effect sfx, int manDir);

extern void mazeManAttack(tile grid[][gridJ],int mazeManDir, int previousDir);

//used for getting the user direction for the MazeMan
extern void getUserInput(tile grid[][gridJ],int &direction, int &prevDir, int held);

extern int pauseGame();

extern void printScreen(tile grid[][gridJ]);

extern void animateDual(tile grid[][gridJ],item *&T1,item *&T2, double delay );

extern void animateDual(tile grid[][gridJ],item *&T1, item *&T2, double delay, mm_sound_effect sfx );

extern void animateItem(tile grid[][gridJ],item &T, double delay, mm_sound_effect sfx );

extern void animateItem(tile grid[][gridJ],item *&T, double delay, mm_sound_effect sfx );

extern void animateItem(tile grid[][gridJ],item *&T, double delay );

extern void animateItem(tile grid[][gridJ],item &T, double delay );

extern void pullSwitch(tile grid[][gridJ],int switchID);

extern void checkSwitch(tile grid[][gridJ], int held,  int I, int J, int dir, int buttonID, int button2ID);

//check actions for button presses and the like
extern int checkActions(tile grid[][gridJ], int currentI, int currentJ, int direction, int previousDir, int held);

//moves the enemy tiles, relevant collision detection
extern void actorMove(tile grid[][gridJ],actor &T, int direction, int manDir, mm_sound_effect sfx);

//moves the player and performs the relevant collision detection etc
extern void actorMove(tile grid[][gridJ],player &T, int direction);

extern void intactCheck(tile grid[][gridJ], int I, int J,int &checkUp, int &checkDown, int &checkLeft, int &checkRight);

extern void wallBreak(tile grid[][gridJ], int &mazeI, int &mazeJ, int dir, int &visitedcells);

extern void setWallSprites(tile grid[][gridJ]);

extern void makeMaze(tile grid[][gridJ], bool braid);

extern void showZombie(tile grid[][gridJ],int level);

extern void showZombieDead(tile grid[][gridJ],int level);

//level two, 2 zombies, 1 door, gold
extern int populateLevelTwo(tile grid[][gridJ]);

//level two, 3 zombies, 2 door, gold
extern int populateLevelThree(tile grid[][gridJ]);

//level one, 1 door, 1 zombie, gold
extern int populateLevelOne(tile grid[][gridJ]);

//adds level Five, crashy crash crash cave in
extern int populateLevelFive(tile grid[][gridJ]);

//level four, 5 zombies,1 blue door, 2 orange
extern int populateLevelFour(tile grid[][gridJ]);

extern void loadPalette();

extern void loadIconsToMemory();

extern int level(tile grid[][gridJ], int level);

#endif