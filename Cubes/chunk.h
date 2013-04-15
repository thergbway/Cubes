#pragma once
#include "world.h"
#include "block.h"
#include "defines.h"

class World;
class Block;

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
private:
};