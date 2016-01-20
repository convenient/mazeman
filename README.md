# MazeMan
## A procedurally generated dungeon game for the Nintendo DS
You take control of MazeMan. Escape the 5 dungeons. Open colour coded doors. Grab some gold. Kill zombies.

<img  style="float: left;" src="https://github.com/convenient/mazeman/raw/master/README/on-console.jpg" height="300"> <img style="float: right;" src="https://github.com/convenient/mazeman/raw/master/README/zoom-game.png" height="300">

## Playing

  1. Download the [.nds](https://github.com/convenient/mazeman/raw/master/src/src.nds) file.
  1. Fire up your favourite Nintendo DS emulator, or pop it on your R4 flash cartridge and play away.

This game used to play fine on Desumume on Windows XP. However it occasionally crashes my Ubuntu 15.10 machine. Probably due to my extremely naive and CPU hungry game loop. The R4 flash cartridge on the actual hardware seems to work reliably.

For more information on this insanity see "[What was I thinking](#what-was-i-thinking)?"

## What was I thinking?

I started making this game at the tail end of 2010 when it was a command line game on Windows. 

<img src="https://github.com/convenient/mazeman/raw/master/README/command-line-2010.png" height="300">

The original C source for this game was converted into C++ to be compatible with devkitARM and libNDS and that's when the complexities started to creep in. 

This was all way before I knew what to do about the following things:

* Version control - I pulled this code from a directory path on an old hard drive like `./devkitPro/C++/nds/new/test/new`. Good times.
* Compiler warnings - If you want to see some funny and easily fixed compilation problems look at [src/make.log](https://github.com/convenient/mazeman/blob/master/src/make.log).
* Memory management - *"Oh yeah, let's allocate every icon I need in the entire game when loading up the ROM boot screen. I'm bound to have enough RAM available."* Clever stuff past Luke, [clever stuff](https://github.com/convenient/mazeman/blob/master/src/source/main.cpp#L579).
* Single Responsibility Principle - A 4520 line long [src/main.cpp](https://github.com/convenient/mazeman/blob/master/src/source/main.cpp) file? What's going on in there! Oh yeah, **EVERYTHING**.
* Basically anything about software engineering, design patterns, reusability. Hell, I even [derived](https://github.com/convenient/mazeman/blob/master/src/source/main.cpp#L1134) [dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) for this game then felt cheated when I found out it was invented 50 odd years previously.

The only reasonable decisions I made in this project were

* Calling him MazeMan. It's a pretty obvious homage to Jumpman, which is what Mario was originally called in Donkey Kong.
* Reading the [Pac-Man Dossier](http://www.gamasutra.com/view/feature/3938/the_pacman_dossier.php?print=1).
* Finishing it. It's a game with 5 levels an a kind-of boss level at the end. A lot of games never get it that far. 

## Compiling

    cd src/
    rm -rf build
    export DEVKITPRO="/home/luker/src/mazeman/devkitpro/"
    export DEVKITARM=${DEVKITPRO}/devkitARM
    make
    //Ignore the compiler warnings and feel free to judge younger, past me.

## Contributing

This code is primarily on GitHub as a monument to my sins.

I'd like to keep the `master` branch as-is, a time-capsule from Spring 2011.

However, if you feel the desire to tidy up some of the mess then feel free to make a pull request to the `develop` branch.
