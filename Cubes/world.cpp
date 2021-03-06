#pragma once
#include <QDebug>
#include "world.h"
#include "gameMain.h"
#include "defines.h"
#include "worldlayerholder.h"
#include "gameDataPreloader.h"

World::World(GameMain* gameMainPtr)
	:gameMain(gameMainPtr){

	//��������� �������������
	gameDataPreloader=new GameDataPreloader(this);
	gameDataPreloaderPtr=gameDataPreloader;
	qDebug()<<"init adresses from world"<<gameDataPreloader<<gameDataPreloaderPtr;
	mainUpdThreadStarterNeedToWait=false;//launch

	//����� ������� ���� � ����� (0;0)
	const int mainChX=0;
	const int mainChZ=0;

	//������� ��� � ������� ������������
	for(int x=0; x<CHUNKS_COUNT; ++x){
		for(int z=0; z<CHUNKS_COUNT; ++z){
			chunks[x][z]=gameDataPreloader->getNewChunkPtr(mainChX + x*BLOCK_COUNT*CUBE_SIZE,mainChZ + z*BLOCK_COUNT*CUBE_SIZE);
		}
	}

	//��������� ��������� ������� �������� ������
	for(int i=0; i<CHUNKS_COUNT; ++i)
		for(int j=0; j<CHUNKS_COUNT; ++j)
			chunkUpdateMatrix[i][j]=true;
}

GameDataPreloader* World::getGameDataPreloaderPtr(){
	return gameDataPreloader;
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
						chunks[i][j]=gameDataPreloader->getNewChunkPtr(mainChX+i*BLOCK_COUNT*CUBE_SIZE,mainChZ+j*BLOCK_COUNT*CUBE_SIZE);
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

	//����� �����(�����������!) ������� ������� ���������� ������ ��� ��������������
	gameDataPreloader->setPlEstCoordinates(gameMain->player->getCoorX(),gameMain->player->getCoorZ());
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

BlockTransparencyAround World::getBlockTransparencyAround(int chNumX,int chNumZ,int blX,int blY,int blZ){
	return BlockTransparencyAround(
		blY<BLOCK_HEIGHT_COUNT-1?chunks[chNumX][chNumZ]->isTransparent(blX,blY+1,blZ):true,
		blY>0?chunks[chNumX][chNumZ]->isTransparent(blX,blY-1,blZ):false,
		blX>0?chunks[chNumX][chNumZ]->isTransparent(blX-1,blY,blZ):
			(chNumX>0?chunks[chNumX-1][chNumZ]->isTransparent(BLOCK_COUNT-1,blY,blZ):true),
		blX<BLOCK_COUNT-1?chunks[chNumX][chNumZ]->isTransparent(blX+1,blY,blZ):
			(chNumX<(CHUNKS_COUNT-1)?chunks[chNumX+1][chNumZ]->isTransparent(0,blY,blZ):true),
		blZ<BLOCK_COUNT-1?chunks[chNumX][chNumZ]->isTransparent(blX,blY,blZ+1):
			(chNumZ<(CHUNKS_COUNT-1)?chunks[chNumX][chNumZ+1]->isTransparent(blX,blY,0):true),
		blZ>0?chunks[chNumX][chNumZ]->isTransparent(blX,blY,blZ-1):
			(chNumZ>0?chunks[chNumX][chNumZ-1]->isTransparent(blX,blY,BLOCK_COUNT-1):true)
		);
}

void World::setVBOForChunkCreated(int chNumX,int chNumZ){
	chunkUpdateMatrix[chNumX][chNumZ]=0;
}

Chunk* World::getChunkPointer(int chNumX,int chNumZ){
	return chunks[chNumX][chNumZ];
}