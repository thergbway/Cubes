#pragma once
#include <QDebug>
#include "world.h"
#include "gameMain.h"
#include "defines.h"

World::World(GameMain* gameMainPtr)
{
	gameMain=gameMainPtr;
	chunksCounter=1;

	//����� ������� ���� � ����� (0;0)
	const int mainChX=0;
	const int mainChZ=0;

	//������� ��� � ������� ������������
	for(int x=0; x<CHUNKS_COUNT; ++x){
		for(int z=0; z<CHUNKS_COUNT; ++z){
			chunks[x][z]=new Chunk(this,mainChX + x*BLOCK_COUNT*CUBE_SIZE,mainChZ + z*BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
		}
	}

	//��������� ��������� ������� �������� ������
	for(int i=0; i<CHUNKS_COUNT; ++i)
		for(int j=0; j<CHUNKS_COUNT; ++j)
			chunkUpdateMatrix[i][j]=true;
}

void World::updateWorld(){
	//���� ���������� �������� (��������, ��. ����������) ����� �� ������ ������
	int plX=(int)gameMain->player->getCoorX();
	int plZ=(int)gameMain->player->getCoorZ();
	int plChX=plX-(plX % (BLOCK_COUNT*CUBE_SIZE));
	int plChZ=plZ-(plZ % (BLOCK_COUNT*CUBE_SIZE));
	int mainChX=plChX-(CHUNKS_COUNT-1)/2*BLOCK_COUNT*CUBE_SIZE;
	int mainChZ=plChZ-(CHUNKS_COUNT-1)/2*BLOCK_COUNT*CUBE_SIZE;

	//��������, �� ��������� �� ������� ����, ���� ��,
	//�� ������������ ������� ������, ��������������� ��,
	//�������� �����
	if((mainChX != chunks[0][0]->getCoordX())||
		mainChZ != chunks[0][0]->getCoordZ()){
			//�������� ����� ������� ������
			Chunk* newChunks[CHUNKS_COUNT][CHUNKS_COUNT];
			for(int i=0;i<CHUNKS_COUNT;++i)
				for(int j=0;j<CHUNKS_COUNT;++j)
					newChunks[i][j]=nullptr;

			//����������� � �����������������
			for(int chX=0; chX<CHUNKS_COUNT; ++chX){
				for(int chZ=0; chZ<CHUNKS_COUNT; ++chZ){
					//��� ������� �����
					//���� ��� ����
					for(int i=0; i<CHUNKS_COUNT; ++i){
						for(int j=0; j<CHUNKS_COUNT; ++j){
							if(	chunks[i][j]!=nullptr &&
								(mainChX+chX*BLOCK_COUNT*CUBE_SIZE)==chunks[i][j]->getCoordX() &&
								(mainChZ+chZ*BLOCK_COUNT*CUBE_SIZE)==chunks[i][j]->getCoordZ()){
									newChunks[chX][chZ]=chunks[i][j];
									chunks[i][j]=nullptr;//�������� ��������� �� ������ �������
									goto exitOfFors;//���������� ������ �����, �� �� ������
							}
						}
					}
					exitOfFors:;
				}
			}

			//������ �������� ����� �� ������ �������
			for(int i=0; i<CHUNKS_COUNT; ++i){
				for(int j=0; j<CHUNKS_COUNT; ++j){
					if(chunks[i][j]!=nullptr){
						delete chunks[i][j];
						chunks[i][j]=nullptr;
					}
				}
			}

			//������������ ������������� ����� � �������� �������, ������� ����� �����
			for(int i=0; i<CHUNKS_COUNT; ++i){
				for(int j=0; j<CHUNKS_COUNT; ++j){
					chunks[i][j]=newChunks[i][j];
					if(chunks[i][j]==nullptr){
						chunks[i][j]=new Chunk(this,mainChX+i*BLOCK_COUNT*CUBE_SIZE,mainChZ+j*BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
						chunkUpdateMatrix[i][j]=true;
					}
				}
			}

			
	}
	//������� �����
	for(int i=0; i<CHUNKS_COUNT; ++i){
		for(int j=0; j<CHUNKS_COUNT; ++j){
			if(chunks[i][j]->updateChunk()==true)
				chunkUpdateMatrix[i][j]=true;
		}
	}
}

int World::getNewChunkId(){
	return chunksCounter++;
}

int World::random(int min,int max,int seed,int x,int z){
	int n=x+z*57+seed;
	n=(n<<13)^n;
	int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
	return (((2-(nn/1073741824))*100)%(max-min+1)+min);//from min to max
}

int World::getChunkCoordX(int chNumX,int chNumZ){
	return chunks[chNumX][chNumZ]->getCoordX();
}

int World::getChunkCoordZ(int chNumX, int chNumZ){
	return chunks[chNumX][chNumZ]->getCoordZ();
}

int World::getChunkId(int chNumX,int chNumZ){
	return chunks[chNumX][chNumZ]->getId();
}

bool World::isChunkUpdated(int chNumX,int chNumZ){
	return chunkUpdateMatrix[chNumX][chNumZ];
}

void World::setVBOForChunkCreated(int chNumX,int chNumZ){
	chunkUpdateMatrix[chNumX][chNumZ]=0;
}

Chunk* World::getChunkPointer(int chNumX,int chNumZ){
	return chunks[chNumX][chNumZ];
}