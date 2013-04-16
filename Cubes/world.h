#pragma once
#include "gameMain.h"
#include "chunk.h"
#include "defines.h"
#include "blocktransparencyaround.h"
#include "worldlayerholder.h"

class GameMain;
class Chunk;
class BlockTransparencyAround;
class WorldLayerHolder;

class World
{
	//members
private:
	GameMain* gameMain;
	WorldLayerHolder* worldLayerHolder;
	Chunk* chunks[CHUNKS_COUNT][CHUNKS_COUNT];
	bool chunkUpdateMatrix[CHUNKS_COUNT][CHUNKS_COUNT];//���������� ������������� World � Graphics
				//-World ������ ��������, Graphics �� �������
	//functions
	int getNewChunkId();
public:
	World(GameMain* gameMainPtr);
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