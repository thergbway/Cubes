#pragma once
#include "chunk.h"
#include "world.h"
#include "worldlayerholder.h"
#include "VBOBox.h"
#include "VBOBoxPrebuild.h"
#include "VBOBoxTransferInfo.h"

class Chunk;
class GameDataPreloader;
class World;
class VBOBox;
class GameMain;
class VBOBoxPrebuild;

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

const int ONE_CHUNK=1;
const int VBOBOXPREBUILD_LONG_SIDE=CHUNKS_TO_DRAW+2*ONE_CHUNK;
const int VBOBOXPREBUILD_SHORT_SIDE=CHUNKS_TO_DRAW;
const int VBOBOXPREBUILD_ENTIRE_LENGTH=2*VBOBOXPREBUILD_LONG_SIDE+2*VBOBOXPREBUILD_SHORT_SIDE;


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
	VBOBoxPrebuild* vBOBoxPrebuild[VBOBOXPREBUILD_ENTIRE_LENGTH];//������ ��������������� VBOBoxPrebuild
	//functions
public:
	GameDataPreloader(World* world);
	~GameDataPreloader();
	Chunk* getNewChunkPtr(int coordX, int coordZ);//�������. ������� � ������������� ����
	VBOBox* getNewVBOBoxPtr(int chNumX,int chNumZ,GameMain* gameMain,GLuint textures[TEXTURES_COUNT]);//�������. ������� � ������������� VBOBox
	void setPlEstCoordinates(double plEstCoordX, double plEstCoordZ);
	void mainUpdCycle();
private:
	void pauseMainUpdCycle();
	void unpauseMainUpdCycle();
	int getNewChunkId();
	double getPlEstCoordX();
	double getPlEstCoordZ();
};