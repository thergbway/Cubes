#pragma once
#include "defines.h"
#include "gameMain.h"

class LayerTransfer;

static const int STONE_MIN=25;//���. ������ �����
static const int STONE_MAX=38;//����. ������ �����(�� ���� �� ����� ��������)
static const int DIRT_MIN=39;//���. ������ �����
static const int DIRT_MAX=114;//����. ������ �����
static const int WATER_LEVEL=65;//���� ����� ������ � �� ���� ������ ������ ������ �����
static const int SAND_STEAK=8;//��������� ����� ����� � ������
static const int SNOW_LEVEL=110;//���� ����� ������ � �� ��� �����->����
static const int TREE_MIN_HEIGHT=70;//������� ����� ���� ���� ��������
static const int TREE_MAX_HEIGHT=105;//������� ����� ���� ����� ��������
static const int TREE_FREQUENCY=50;//������� ��������� ������ N �� 100
static const int TREE_HEIGHT_MAX=12;//����. ������ ������
static const int TREE_HEIGHT_MIN=8;//���. ������ ������
static const int TREE_OFFSET=1;//�������� ������������ ������. ���� ������ ������, ���� ��� ������ �� �������� ������
static const int TREE_MAX_WIDTH=3;//������������ ������ ������. �� ��������
static const int STONE_SPARPNESS=55;//������������� ���� �����, ��� ������, ��� �����
static const int DIRT_SPARPNESS=20;//������������� ���� �����, ��� ������, ��� �����

//����� ������������ ��� ��������� ����������
//� ������ ����� ��� ��������� ���������� � ������
//����������� ����� ���������� ��� ���������� ��� ��������� ���������
class WorldLayerHolder{
	//variables
private:
	static const int seed=SEED;
	byte stoneLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//�������� ����� ��������� ������
	byte dirtLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//�������� ����� ��������� ������
	byte waterLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//�������� ����� ������� ������
	byte sandLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//�������� ����� ��������� ������
	byte snowLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//�������� ����� �������� ������
	byte woodLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//�������� ����� ������� ��������
	int coordX;//���������� �����(��. ����������), ������� ���������� ������������ �������
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

class LayerTransfer{//����� ��� �������� ����
public:
	byte stoneLayer[BLOCK_COUNT][BLOCK_COUNT];
	byte dirtLayer[BLOCK_COUNT][BLOCK_COUNT];
	byte waterLayer[BLOCK_COUNT][BLOCK_COUNT];
	byte sandLayer[BLOCK_COUNT][BLOCK_COUNT];
	byte snowLayer[BLOCK_COUNT][BLOCK_COUNT];
	byte woodLayer[BLOCK_COUNT][BLOCK_COUNT];
};