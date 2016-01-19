
//{{BLOCK(instructions)

//======================================================================
//
//	instructions, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ bitmap not compressed
//	Total size: 512 + 65536 = 66048
//
//	Time-stamp: 2016-01-19, 16:56:58
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.13
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_INSTRUCTIONS_H
#define GRIT_INSTRUCTIONS_H

#define instructionsBitmapLen 65536
extern const unsigned int instructionsBitmap[16384];

#define instructionsPalLen 512
extern const unsigned short instructionsPal[256];

#endif // GRIT_INSTRUCTIONS_H

//}}BLOCK(instructions)
