#pragma once
#include "defines.h"
#include "gameMain.h"

class LayerTransfer;

//����� ������������ ��� ��������� ����������
//� ������ ����� ��� ��������� ���������� � ������
//����������� ����� ���������� ��� ���������� ��� ��������� ���������

class WorldLayerHolder{
	//variables
private:
	static const int seed=SEED;
	static const int STONE_MIN=2;//���. ������ �����
	static const int STONE_MAX=58;//����. ������ �����(�� ���� �� ����� ��������)
	static const int DIRT_MIN=0;//���. ������ �����
	static const int DIRT_MAX=28;//����. ������ �����
	static const int STONE_SPARPNESS=20;//������������� ���� �����
	static const int DIRT_SPARPNESS=45;//������������� ���� �����
	byte stoneLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//�������� ����� ��������� ������
	byte dirtLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];//�������� ����� ��������� ������
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
};