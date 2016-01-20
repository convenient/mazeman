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
#include "tile.h"
#include "item.h"
#include "actor.h"
#include "player.h"
#include "sound.h"
#include "level.h"
#include "input.h"

using namespace std;

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
    int released;
    do {
        scanKeys();
        released = keysHeld();
    } while(released & KEY_A);

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

        if(held & KEY_UP){
            if(position > 0){
                position--;
                mmEffectEx(&ladderSound);
            }
        } 
        else if(held & KEY_DOWN) {
            if(position < 2){
                position++;
                mmEffectEx(&ladderSound);
            }
        }

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
    tileMazeMan.make(tileMazeManID,solidPlayer,animManDown,mode);

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

    vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06000000,
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
