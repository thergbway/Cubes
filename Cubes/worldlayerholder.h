#pragma once
#include "defines.h"
#include "gameMain.h"

class LayerTransfer;

static const int STONE_MIN=25;//мин. высота камня
static const int STONE_MAX=38;//макс. высота камня(от нуля до этого значения)
static const int DIRT_MIN=39;//мин. высота земли
static const int DIRT_MAX=114;//макс. высота земли
static const int WATER_LEVEL=65;//ниже этого уровня и на этой высоте воздух станет водой
static const int SAND_STEAK=8;//прослойка между водой и землей
static const int SNOW_LEVEL=110;//выше этого уровня и на нем земля->снег
static const int TREE_MIN_HEIGHT=70;//деревья будут выше того значения
static const int TREE_MAX_HEIGHT=105;//деревья будут ниже этого значения
static const int TREE_FREQUENCY=50;//частота появления дерева N из 100
static const int TREE_HEIGHT_MAX=12;//макс. высота дерева
static const int TREE_HEIGHT_MIN=8;//мин. высота дерева
static const int TREE_OFFSET=1;//смещение относительно центра. Куда увести дерево, чтоб они стояли не строгими рядами
static const int TREE_MAX_WIDTH=3;//максимальная ширина дерева. Дб нечетное
static const int STONE_SPARPNESS=55;//шероховатость слоя камня, чем больше, тем глаже
static const int DIRT_SPARPNESS=20;//шероховатость слоя земли, чем больше, тем глаже

//класс используется для получения информации
//о высоте слоев для различных материалов в чанках
//конструктор чанка использует эту информацию при генерации ландшафта
class WorldLayerHolder{
	//variables
private:
	static const int seed=SEED;
	byte stoneLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//значения высот каменного уровня
	byte dirtLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//значения высот каменного уровня
	byte waterLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//значения высот водного уровня
	byte sandLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//значения высот песчаного уровня
	byte snowLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//значения высот снежного уровня
	byte woodLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//значения высот стволов деревьев
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
	byte waterLayer[BLOCK_COUNT][BLOCK_COUNT];
	byte sandLayer[BLOCK_COUNT][BLOCK_COUNT];
	byte snowLayer[BLOCK_COUNT][BLOCK_COUNT];
	byte woodLayer[BLOCK_COUNT][BLOCK_COUNT];
};