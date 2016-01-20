#ifndef SOUND_H
#define SOUND_H

#include <nds.h>
#include <maxmod9.h>

#include "soundbank.h"
#include "soundbank_bin.h"

extern mm_sound_effect keyPickupSound, doorOpenSound, ladderSound, doorStuck, swordSound, zombieMoan, rockFall, hurt;

extern void initSounds();

#endif