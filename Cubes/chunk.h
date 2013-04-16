#pragma once
#pragma warning(disable: 4290)
#include <new>
#include <cstddef>//size_t
#include "world.h"
#include "block.h"
#include "defines.h"
#include "worldlayerholder.h"

class World;
class Block;
class LayerTransfer;
class WorldLayerHolder;

class Chunk
{
	//members
private:
	World* world;
	int id;
	int coordX;//координата точки (см. соглашение) чанка
	int coordZ;//тоже
	static bool alloc_map[];//для динамического выделения памяти
	static unsigned char pool[];//для динамического выделения памяти
public:
	Block blocks[BLOCK_COUNT][BLOCK_HEIGHT_COUNT][BLOCK_COUNT];
	//functions
public:
	Chunk(World* worldPtr,WorldLayerHolder* worldLayerHolder, int coordX, int coordZ, int id);
	void* operator new(size_t) throw(std::bad_alloc);
	void operator delete(void*);
	int getCoordX();
	int getCoordZ();
	int getId();
	bool updateChunk();//1-changed,0-no change
	bool isTransparent(int blX,int blY,int blZ);
private:
};