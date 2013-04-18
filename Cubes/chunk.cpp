#pragma once
#include <stdlib.h>
#include <crtdbg.h>
#include <math.h>
#include <QDebug>
#include "chunk.h"
#include "world.h"
#include "worldlayerholder.h"
#include "gameDataPreloader.h"
#include "defines.h"

const int CHUNKS_HEAP_SIZE=CHUNKS_COUNT*CHUNKS_COUNT+CHUNKS_PRELOAD_ENTIRE_SIDE_LENGTH*CHUNKS_PRELOAD_BORDER;
bool Chunk::alloc_map[CHUNKS_HEAP_SIZE];//для динамического выделения памяти
unsigned char Chunk::pool[CHUNKS_HEAP_SIZE*sizeof(Chunk)];//для динамического выделения памяти

Chunk::Chunk(World* worldPtr,WorldLayerHolder* worldLayerHolder,int _coordX, int _coordZ, int _id)
{
	//тут ничего не должно быть- инициализируем все в if
	if(CHOSE_LANDSCAPE==TYPE1){//random landscape that is less smoothed
		world=worldPtr;
		coordX=_coordX;
		coordZ=_coordZ;
		id=_id;

		//creating chunk
		for(int x=0; x<BLOCK_COUNT; ++x){
			for (int z=0; z<BLOCK_COUNT; ++z){
				for(int y=0; y<BLOCK_HEIGHT_COUNT; ++y){
					blocks[x][y][z].setAir();
				}
			}
		}
		//setDirt random.
		const int MAX_DIRT_HEIGHT=200;
		const int MIN_DIRT_HEIGHT=10;

		int matrix[BLOCK_COUNT][BLOCK_COUNT];

		for(int i=0;i<BLOCK_COUNT;++i)
			for(int j=0;j<BLOCK_COUNT;++j)
				matrix[i][j]=(rand()%MAX_DIRT_HEIGHT)+1+MIN_DIRT_HEIGHT;

		//сглаживание
		//big
		const int B_SMOOTH=32;
		const int B_DIVIDER=36;
		for(int s=0; s<B_SMOOTH; ++s){
			int average=0;
			for(int i=0;i<BLOCK_COUNT;++i){
				for(int j=0;j<BLOCK_COUNT;++j){
					average+=matrix[i][j];
				}
			}
			average/=BLOCK_COUNT*BLOCK_COUNT;
			for(int i=0;i<BLOCK_COUNT;++i){
				for(int j=0;j<BLOCK_COUNT;++j){
					if(matrix[i][j]>average)
						matrix[i][j]-=average/B_DIVIDER;
					else
						if(matrix[i][j]<average)
							matrix[i][j]+=average/B_DIVIDER;
				}
			}
		}

		//small
		const int SMOOTH=7;
		const int DIVIDER=8;
		for(int s=0;s<SMOOTH;++s){
			for(int i=0;i<BLOCK_COUNT-1;++i){
				for(int j=0;j<BLOCK_COUNT-1;++j){
					//average
					int average=(matrix[i][j]+matrix[i+1][j]+matrix[i][j+1]+matrix[i+1][j+1])/4;

					if(matrix[i][j]<average)
						matrix[i][j]+=average/DIVIDER;
					else
						if(matrix[i][j]>average)
							matrix[i][j]-=average/DIVIDER;

					if(matrix[i+1][j]<average)
						matrix[i+1][j]+=average/DIVIDER;
					else
						if(matrix[i+1][j]>average)
							matrix[i+1][j]-=average/DIVIDER;

					if(matrix[i][j+1]<average)
						matrix[i][j+1]+=average/DIVIDER;
					else
						if(matrix[i][j+1]>average)
							matrix[i][j+1]-=average/DIVIDER;

					if(matrix[i+1][j+1]<average)
						matrix[i+1][j+1]+=average/DIVIDER;
					else
						if(matrix[i+1][j+1]>average)
							matrix[i+1][j+1]-=average/DIVIDER;
				}
			}
		}

		for(int x=0; x<BLOCK_COUNT; ++x){
			for(int z=0; z<BLOCK_COUNT; ++z){
				for(int y=0;y<matrix[x][z];++y){
					blocks[x][y][z].setDirt();
				}
				blocks[x][matrix[x][z]-1][z].setGrass();//верхний слой трава
			}
		}
	}
	if(CHOSE_LANDSCAPE==TYPE2){
		world=worldPtr;
		coordX=_coordX;
		coordZ=_coordZ;
		id=_id;

		//creating chunk
		for(int x=0; x<BLOCK_COUNT; ++x){
			for (int z=0; z<BLOCK_COUNT; ++z){
				for(int y=0; y<BLOCK_HEIGHT_COUNT; ++y){
					blocks[x][y][z].setAir();
				}
			}
		}
		int matrix[BLOCK_COUNT][BLOCK_COUNT];

		//set minimum level of dirt
		const int MIN_DIRT=10;
		const int A1=10;
		const int A2=40;
		const int T1=40;
		const int T2=250;
		const int B1=30;
		const int B2=150;
		for(int i=0;i<BLOCK_COUNT;++i)
			for(int j=0;j<BLOCK_COUNT;++j){
				int blX=coordX/CUBE_SIZE+i;
				int blZ=coordZ/CUBE_SIZE+j;
				matrix[i][j]=MIN_DIRT+A1+A1*sin(2*PI/T1*blZ+B1)+A2+A2*sin(2*PI/T2*blX+B2);
			}
		for(int x=0; x<BLOCK_COUNT; ++x){
			for(int z=0; z<BLOCK_COUNT; ++z){
				for(int y=0;y<matrix[x][z];++y){
					blocks[x][y][z].setDirt();
				}
				blocks[x][matrix[x][z]-1][z].setGrass();//верхний слой трава
			}
		}
	}
	if(CHOSE_LANDSCAPE==TYPE3){
		world=worldPtr;
		coordX=_coordX;
		coordZ=_coordZ;
		id=_id;

		//creating chunk
		//filling with air
		for(int x=0; x<BLOCK_COUNT; ++x){
			for (int z=0; z<BLOCK_COUNT; ++z){
				for(int y=0; y<BLOCK_HEIGHT_COUNT; ++y){
					blocks[x][y][z].setAir();
				}
			}
		}
		//getting LayerTransfer object
		LayerTransfer layerTransfer;
		worldLayerHolder->updateLayerTransferForChunk(_coordX,_coordZ,layerTransfer);

		//filling with stone
		for(int x=0; x<BLOCK_COUNT; ++x)
			for(int z=0; z<BLOCK_COUNT; ++z)
				for(int y=0; y<layerTransfer.stoneLayer[x][z]; ++y){
					if(y+1 != layerTransfer.stoneLayer[x][z])
						blocks[x][y][z].setDirt();
					else
						blocks[x][y][z].setGrass();
				}
	}
	//тут ничего не должно быть- инициализируем все в if
}

Chunk::~Chunk(){
}

void* Chunk::operator new(size_t) throw(std::bad_alloc){
	for(int i=0; i<CHUNKS_HEAP_SIZE; ++i){
		if(!alloc_map[i]){
			alloc_map[i]=true;//mark as in use
			void* toReturn=pool+i*sizeof(Chunk);
			return toReturn;
		}
	}
	//if space has not been found
	qDebug()<<"No more free memory to allocate from pool. See implementation of class Chunk. Throwing std::bad_alloc.";
	throw std::bad_alloc();
}

void Chunk::operator delete(void* ptr){
	if(!ptr){
		qDebug()<<"function Chunk::operator delete was called with NULL parameter. Check for error.";
		return;
	}
	//вычислим номер блока из пула
	unsigned long long block=(unsigned long long) ptr- (unsigned long long)pool;
	block /= sizeof(Chunk);
	//mark it free
	alloc_map[block]=false;
}

int Chunk::getCoordX(){
	return coordX;
}

int Chunk::getCoordZ(){
	return coordZ;
}

int Chunk::getId(){
	return id;
}

bool Chunk::updateChunk(){
	return 0;
}

bool Chunk::isTransparent(int blX,int blY,int blZ){
	return blocks[blX][blY][blZ].isTransparent();
}