#pragma once
#include "world.h"
#include "block.h"
#include "defines.h"

class World;
class Block;
class BlockTransparencyAround;

class Chunk
{
	//members
private:
	World* world;
	int id;
	int coordX;//координата точки (см. соглашение) чанка
	int coordZ;//тоже
public:
	Block blocks[BLOCK_COUNT][BLOCK_HEIGHT_COUNT][BLOCK_COUNT];
	//functions
public:
	Chunk(World* worldPtr, int coordX, int coordZ, int id);
	int getCoordX();
	int getCoordZ();
	int getId();
	bool updateChunk();//1-changed,0-no change
	BlockTransparencyAround getBlockTransparencyAround(int blX,int blY,int blZ);
	bool isTransparent(int blX,int blY,int blZ);
private:
};

class BlockTransparencyAround{
public:
	bool top;
	bool down;
	bool left;
	bool right;
	bool front;
	bool back;
	BlockTransparencyAround(bool top,bool down,bool left,bool right,bool front,bool back);
};