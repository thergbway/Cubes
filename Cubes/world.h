#pragma once
#include "gameMain.h"
#include "chunk.h"
#include "defines.h"

class GameMain;
class Chunk;
class BlockTransparencyAround;

class World
{
	//members
private:
	GameMain* gameMain;
	int chunksCounter;
	Chunk* chunks[CHUNKS_COUNT][CHUNKS_COUNT];
	bool chunkUpdateMatrix[CHUNKS_COUNT][CHUNKS_COUNT];//совместная собственность World и Graphics
				//-World ставит единички, Graphics их убирает
	//functions
	int getNewChunkId();
	int random(int min,int max,int seed, int x,int z);
public:
	World(GameMain* gameMainPtr);
	int getChunkCoordX(int chNumX,int chNumZ);
	int getChunkCoordZ(int chNumX,int chNumZ);
	int getChunkId(int chNumX,int chNumZ);
	Chunk* getChunkPointer(int chNumX,int chNumZ);
	bool isChunkUpdated(int chNumX,int chNumZ);
	BlockTransparencyAround getBlockTransparencyAround(int chNumX,int chNumZ,int blX,int blY,int blZ);//получаем объект-прозрачность вокруг текущего блока
	void setVBOForChunkCreated(int chNumX,int chNumZ);//ставит в матрице апдейтов для данного чанка 0
	void updateWorld();//обновляем мир-создаем новые чанки,
						//перераспределяем их, обновляем
};

class BlockTransparencyAround{//прозрачность вокруг блока
public:
	bool top;
	bool down;
	bool left;
	bool right;
	bool front;
	bool back;
	BlockTransparencyAround(bool top,bool down,bool left,bool right,bool front,bool back);
};