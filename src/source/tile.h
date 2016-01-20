#ifndef TILE_H
#define TILE_H

class tile {
  protected:
    int tileID;
  public:
    int solidity;
    int spriteRef;
    int reference;

    void make(int x, int y, int z, int sref);
    void setSolidity(int solid);
    int getID();
    int getSolidity();
    int getRef();
    void resetRef();
    void incRef();
    void setRef(int x);
    void setSprite(int x);
    int getSprite();
};


extern tile tileEmpty, tileWall, tileExplosion, tileRock;

#endif