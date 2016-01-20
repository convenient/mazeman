#include "tile.h"

void tile::make(int x, int y, int z, int sref){
	tileID=x, solidity=y, reference=z, spriteRef=sref;
}
void tile::setSolidity(int solid){
	solidity=solid;
}
int tile::getID(){
	return tileID;
}
int tile::getSolidity(){
	return solidity;
}
int tile::getRef(){
	return reference;
}
void tile::resetRef(){
	reference=0;
}
void tile::incRef(){
	reference++;
}
void tile::setRef(int x){
	reference=x;
}
void tile::setSprite(int x){
	spriteRef=x;
}
int tile::getSprite(){
	return spriteRef;
}

tile tileEmpty, tileWall, tileExplosion, tileRock;