#include "sound.h"

mm_sound_effect keyPickupSound, doorOpenSound, ladderSound, doorStuck, swordSound, zombieMoan, rockFall, hurt;

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