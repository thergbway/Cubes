#pragma once
#include <QDebug>
#include "world.h"
#include "gameMain.h"
#include "defines.h"

World::World(GameMain* gameMainPtr)
{
	gameMain=gameMainPtr;

	//пусть главный чанк в точке (0;0)
	const int mainChX=0;
	const int mainChZ=0;

	//создаем мир с нужными координатами
	for(int x=0; x<CHUNKS_COUNT; ++x){
		for(int z=0; z<CHUNKS_COUNT; ++z){
			chunks[x][z]=new Chunk(this,mainChX + x*BLOCK_COUNT*CUBE_SIZE,mainChZ + z*BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
		}
	}

	//заполняем единицами матрицу апдейтов чанков
	for(int i=0; i<CHUNKS_COUNT; ++i)
		for(int j=0; j<CHUNKS_COUNT; ++j)
			chunkUpdateMatrix[i][j]=true;
}

void World::updateWorld(){
	//ищем координату главного (нулевого, см. соглашение) чанка на данный момент
	int plX=(int)gameMain->player->getCoorX();
	int plZ=(int)gameMain->player->getCoorZ();
	int plChX=plX-(plX % (BLOCK_COUNT*CUBE_SIZE));
	int plChZ=plZ-(plZ % (BLOCK_COUNT*CUBE_SIZE));
	int mainChX=plChX-(CHUNKS_COUNT-1)/2*BLOCK_COUNT*CUBE_SIZE;
	int mainChZ=plChZ-(CHUNKS_COUNT-1)/2*BLOCK_COUNT*CUBE_SIZE;

	//проверим, не изменился ли главный чанк, если да,
	//то верифицируем матрицу чанков, перераспределим их,
	//создадим новые
	if((mainChX != chunks[0][0]->getCoordX())||
		mainChZ != chunks[0][0]->getCoordZ()){
			//создадим новую матрицу чанков
			Chunk* newChunks[CHUNKS_COUNT][CHUNKS_COUNT];
			for(int i=0;i<CHUNKS_COUNT;++i)
				for(int j=0;j<CHUNKS_COUNT;++j)
					newChunks[i][j]=nullptr;

			//верификация и перераспределение
			for(int chX=0; chX<CHUNKS_COUNT; ++chX){
				for(int chZ=0; chZ<CHUNKS_COUNT; ++chZ){
					//для каждого чанка
					//ищем ему пару
					for(int i=0; i<CHUNKS_COUNT; ++i){
						for(int j=0; j<CHUNKS_COUNT; ++j){
							if(	chunks[i][j]!=nullptr &&
								(mainChX+chX*BLOCK_COUNT*CUBE_SIZE)==chunks[i][j]->getCoordX() &&
								(mainChZ+chZ*BLOCK_COUNT*CUBE_SIZE)==chunks[i][j]->getCoordZ()){
									newChunks[chX][chZ]=chunks[i][j];
									chunks[i][j]=nullptr;//забираем указатель из старой матрицы
									goto exitOfFors;//прекратить подбор чанка, тк он найден
							}
						}
					}
					exitOfFors:;
				}
			}

			//удалим ненужные чанки из старой матрицы
			for(int i=0; i<CHUNKS_COUNT; ++i){
				for(int j=0; j<CHUNKS_COUNT; ++j){
					if(chunks[i][j]!=nullptr){
						delete chunks[i][j];
						chunks[i][j]=nullptr;
					}
				}
			}

			//переписываем сохранившиеся чанки в основную матрицу, создаем новые чанки
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
	//обновим чанки
	for(int i=0; i<CHUNKS_COUNT; ++i){
		for(int j=0; j<CHUNKS_COUNT; ++j){
			if(chunks[i][j]->updateChunk()==true)
				chunkUpdateMatrix[i][j]=true;
		}
	}
}

int World::getNewChunkId(){
	static int chunksCounter=0;
	return ++chunksCounter;
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

BlockTransparencyAround::BlockTransparencyAround(bool _top,bool _down,bool _left,bool _right,bool _front,bool _back)
	:top(_top),down(_down),left(_left),right(_right),front(_front),back(_back){}