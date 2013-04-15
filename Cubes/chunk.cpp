#pragma once
#include <stdlib.h>
#include <crtdbg.h>
#include <QDebug>
#include "chunk.h"
#include "world.h"
#include "defines.h"

Chunk::Chunk(World* worldPtr,int _coordX, int _coordZ, int _id)
{
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

	//�����������
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
		}
	}
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