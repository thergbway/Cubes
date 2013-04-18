#pragma once
#include "chunk.h"
#include "world.h"
#include "worldlayerholder.h"

class Chunk;
class GameDataPreloader;
class World;

//������ � �������� ��������������
unsigned int __stdcall mainUpdThreadStarter(void* pParams);
extern bool mainUpdThreadStarterNeedToWait;//���� �������� ������ ������ ��������������
extern GameDataPreloader* gameDataPreloaderPtr;//����� �������������� ����� ������������ ���� ������
//~����� ������ � ���������������

//���������
const int CHUNKS_PRELOAD_BORDER=CHUNKS_PRELOAD_COUNT;
const int CHUNKS_PRELOAD_LONG_SIDE=CHUNKS_PRELOAD_BORDER*2+CHUNKS_COUNT;
const int CHUNKS_PRELOAD_SHORT_SIDE=CHUNKS_COUNT;
const int CHUNKS_PRELOAD_ENTIRE_SIDE_LENGTH=CHUNKS_PRELOAD_LONG_SIDE*2+CHUNKS_PRELOAD_SHORT_SIDE*2;

class GameDataPreloader{
	//����� ������������� ������������� ������ �� ������, VBOBox, WorldLayerHolder.
	//����� �� ����� ����� ����������������� � ������� �����.
	//��� ������ ��� �� �������������. ����� �� ���������� ������ ��� ������
	//� ������ �������� ��������� ������������ �����, ������� � ���������� ���� ������
	//��� ������� ������ ����� �������������. �� ��������� �������� ����� ���������

	//members
public:
private:
	CRITICAL_SECTION plCoordCriSec; // ����������� ������
	World* world;
	WorldLayerHolder* worldLayerHolder;
	double plEstCoordX;//��������� ���������� ������
	double plEstCoordZ;//��������� ���������� ������
	bool mainUpdCyclePaused;//����, �������������� �������
	bool mainUpdCycleRedFlag;//���� ��� ������. ���������� ������������
	Chunk* chunksPreload[CHUNKS_PRELOAD_ENTIRE_SIDE_LENGTH][CHUNKS_PRELOAD_BORDER];
	//functions
public:
	GameDataPreloader(World* world);
	~GameDataPreloader();
	Chunk* getNewChunkPtr(int coordX, int coordZ);//�������. ������� � ������������� ����
	void setPlEstCoordinates(double plEstCoordX, double plEstCoordZ);
	void mainUpdCycle();
private:
	void pauseMainUpdCycle();
	void unpauseMainUpdCycle();
	int getNewChunkId();
	double getPlEstCoordX();
	double getPlEstCoordZ();
};