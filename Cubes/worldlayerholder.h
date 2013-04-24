#pragma once
#include "defines.h"
#include "gameMain.h"

class LayerTransfer;

//класс используется для получения информации
//о высоте слоев для различных материалов в чанках
//конструктор чанка использует эту информацию при генерации ландшафта

class WorldLayerHolder{
	//variables
private:
	static const int seed=SEED;
	static const int STONE_MIN=2;//мин. высота камня
	static const int STONE_MAX=58;//макс. высота камня(от нуля до этого значения)
	static const int DIRT_MIN=0;//мин. высота земли
	static const int DIRT_MAX=28;//макс. высота земли
	static const int STONE_SPARPNESS=20;//шероховатость слоя камня
	static const int DIRT_SPARPNESS=45;//шероховатость слоя земли
	byte stoneLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//значения высот каменного уровня
	byte dirtLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//значения высот каменного уровня
	int coordX;//коодринаты точки(см. соглашение), которая определяет подгруженную область
	int coordZ;
public:

	//functions
private:
	int random(int min,int max,int seed, int x,int z);
	bool chance(int first,int ofSecond,int seed, int x, int z);
	void reloadLayers(int coordMainX, int coordMainZ);
	void stoneMakeSquare(int indexLeftBackX,int indexLeftBackZ,int indexRightFrontX, int indexRightFrontZ, float stoneTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT]);//square step for stone layer
	void stoneMakeDiamond(int indexBackX,int indexBackZ,int indexRightX, int indexRightZ,float stoneTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT]);//diamond step for stone layer
	void dirtMakeSquare(int indexLeftBackX,int indexLeftBackZ,int indexRightFrontX, int indexRightFrontZ, float dirtTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT]);//square step for dirt layer
	void dirtMakeDiamond(int indexBackX,int indexBackZ,int indexRightX, int indexRightZ,float dirtTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT]);//diamond step for dirt layer
public:
	WorldLayerHolder();
	void updateLayerTransferForChunk(int coorX,int coorZ,LayerTransfer& outputLayerTransfer);//changes outputLayerTransfer according to the requesting chunk
};

class LayerTransfer{//класс для передачи слоёв
public:
	byte stoneLayer[BLOCK_COUNT][BLOCK_COUNT];
	byte dirtLayer[BLOCK_COUNT][BLOCK_COUNT];
};