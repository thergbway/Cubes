#pragma once
#include "gameMain.h"
#include "chunk.h"
#include "defines.h"
#include "blocktransparencyaround.h"
#include "gameDataPreloader.h"

class GameMain;
class Chunk;
class BlockTransparencyAround;
class GameDataPreloader;

class World
{
	//members
private:
	GameMain* gameMain;
	GameDataPreloader* gameDataPreloader;
	Chunk* chunks[CHUNKS_COUNT][CHUNKS_COUNT];
	bool chunkUpdateMatrix[CHUNKS_COUNT][CHUNKS_COUNT];//���������� ������������� World � Graphics
				//-World ������ ��������, Graphics �� �������
	//functions
public:
	World(GameMain* gameMainPtr);
	GameDataPreloader* getGameDataPreloaderPtr();
	int getChunkCoordX(int chNumX,int chNumZ);
	int getChunkCoordZ(int chNumX,int chNumZ);
	int getChunkId(int chNumX,int chNumZ);
	Chunk* getChunkPointer(int chNumX,int chNumZ);
	bool isChunkUpdated(int chNumX,int chNumZ);
	BlockTransparencyAround getBlockTransparencyAround(int chNumX,int chNumZ,int blX,int blY,int blZ);//�������� ������-������������ ������ �������� �����
	void setVBOForChunkCreated(int chNumX,int chNumZ);//������ � ������� �������� ��� ������� ����� 0
	void updateWorld();//��������� ���-������� ����� �����,
						//���������������� ��, ���������
};