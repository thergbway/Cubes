#pragma once
#include "defines.h"
#include "gameDataPreloader.h"

bool mainUpdThreadStarterNeedToWait;//флаг ожидания старта потока предзагрузчика
GameDataPreloader* gameDataPreloaderPtr;//поток предзагрузчика будет использовать этот объект

unsigned int __stdcall mainUpdThreadStarter(void* pParams){
	qDebug()<<"thread initialized";
	while(mainUpdThreadStarterNeedToWait)
		;//ждем старта
	qDebug()<<"thread started";
	GameDataPreloader* gameDataPreloader=gameDataPreloaderPtr;	
	if(gameDataPreloader == nullptr)
		qDebug()<<"gameDataPreloader.cpp mainUpdThreadStarter got nullptr as gameDataPreloaderPtr. Cannot be continued";
	qDebug()<<"init adresses from thread"<<gameDataPreloader<<gameDataPreloaderPtr;
	gameDataPreloader->mainUpdCycle();//запустили цикл

	return 0;
}

GameDataPreloader::GameDataPreloader(World* worldPtr)
	:world(worldPtr){
		// Инициализируем критическую секцию
		InitializeCriticalSection(&plCoordCriSec);
		worldLayerHolder=new WorldLayerHolder;
		plEstCoordX=0;
		plEstCoordZ=0;
		mainUpdCyclePaused=false;
		mainUpdCycleRedFlag=false;

		//сбросим массив чанков
		for(int i=0; i<CHUNKS_PRELOAD_ENTIRE_SIDE_LENGTH; ++i)
			for(int j=0; j<CHUNKS_PRELOAD_BORDER; ++j)
				chunksPreload[i][j]=nullptr;
		//and one else
		for(int i=0; i<CHUNKS_PRELOAD_SUB_ENTIRE_LENGTH; ++i)
			chunksSubPreload[i]=nullptr;
		//сбросим массив VBOBoxPrebuilds
		for(int i=0;i<VBOBOXPREBUILD_PRELOAD_COUNT;++i)
			vBOBoxPrebuilds[i]=nullptr;
}

GameDataPreloader::~GameDataPreloader(){
	// Удаляем критическую секцию
	DeleteCriticalSection(&plCoordCriSec);
}

Chunk* GameDataPreloader::getNewChunkPtr(int coordX, int coordZ){
	pauseMainUpdCycle();
	
	for(int i=0; i<CHUNKS_PRELOAD_ENTIRE_SIDE_LENGTH; ++i){
		for(int j=0; j<CHUNKS_PRELOAD_BORDER; ++j){
			Chunk* currChPtr=chunksPreload[i][j];
			if(currChPtr != nullptr &&
				currChPtr->getCoordX() == coordX &&
				currChPtr->getCoordZ() == coordZ){
					//этот чанк подходит
					chunksPreload[i][j]=nullptr;
					unpauseMainUpdCycle();
					return currChPtr;
			}
		}
	}
	//чанк не был найден(не предзагружен)
	Chunk* toReturn=new Chunk(world,worldLayerHolder,coordX,coordZ,getNewChunkId());
	//qDebug()<<"cannot find chunk in preloads x="<<coordX<<"z="<<coordZ;
	unpauseMainUpdCycle();
	return toReturn;

}

VBOBox* GameDataPreloader::getNewVBOBoxPtr(int chNumX,int chNumZ,GameMain* gameMain,GLuint textures[TEXTURES_COUNT]){
	return new VBOBox(chNumX,chNumZ,gameMain,textures);
}

void GameDataPreloader::pauseMainUpdCycle(){
	mainUpdCycleRedFlag=true;
	while(!mainUpdCyclePaused)
		mainUpdCycleRedFlag=true;//assign and wait
}

void GameDataPreloader::unpauseMainUpdCycle(){
	mainUpdCycleRedFlag=false;
	//while(mainUpdCyclePaused)
		//mainUpdCycleRedFlag=false;//assign and wait for start
}

int GameDataPreloader::getNewChunkId(){
	static int chunksCounter=0;
	return ++chunksCounter;
}

void GameDataPreloader::setPlEstCoordinates(double _plEstCoordX, double _plEstCoordZ){
	EnterCriticalSection(&plCoordCriSec); // Заходим в к.с. (ждем входа)
	plEstCoordX=_plEstCoordX;
	plEstCoordZ=_plEstCoordZ;
	LeaveCriticalSection(&plCoordCriSec); // Выходим из к.с.
}

void GameDataPreloader::mainUpdCycle(){
	qDebug()<<"mainUpdCycle works";

	int oldMainChX=115468165;//фиктивная координата(случайное число)
	int oldMainChZ=-11256148;//фиктивная координата(случайное число)

	while(true){//выполнять вечно
		if(!mainUpdCycleRedFlag){//можно выполнять
			mainUpdCyclePaused=false;

			//выполняем предзагрузку чанков
			//ищем координату главного (нулевого, см. соглашение) чанка на данный момент
			const int plX=(int)getPlEstCoordX();
			const int plZ=(int)getPlEstCoordZ();
			const int plChX=plX-(plX % (BLOCK_COUNT*CUBE_SIZE));
			const int plChZ=plZ-(plZ % (BLOCK_COUNT*CUBE_SIZE));
			const int mainChX=plChX-(CHUNKS_COUNT-1)/2*BLOCK_COUNT*CUBE_SIZE;
			const int mainChZ=plChZ-(CHUNKS_COUNT-1)/2*BLOCK_COUNT*CUBE_SIZE;

			if(oldMainChX != mainChX || oldMainChZ != mainChZ){
				//если координаты главного чанка изменились

				//MAIN CHUNKS
				//update back side chunks
				const int mainBackChX=mainChX-CHUNKS_PRELOAD_BORDER*BLOCK_COUNT*CUBE_SIZE;
				const int mainBackChZ=mainChZ-CHUNKS_PRELOAD_BORDER*BLOCK_COUNT*CUBE_SIZE;
				for(int i=0; i<CHUNKS_PRELOAD_LONG_SIDE; ++i){
					for(int j=0; j<CHUNKS_PRELOAD_BORDER; ++j){

						if(mainUpdCycleRedFlag)
							goto emergencyExit;//экстренный выход, если приказано остановить поток

						//find current coordinates
						int currChX=mainBackChX+i*BLOCK_COUNT*CUBE_SIZE;
						int currChZ=mainBackChZ+j*BLOCK_COUNT*CUBE_SIZE;
						if(chunksPreload[i][j] != nullptr)//если там что-то было
							delete chunksPreload[i][j];
						chunksPreload[i][j]=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
					}
				}//finished back side updating

				//update left side chunks
				const int mainLeftChX=mainChX+CHUNKS_COUNT*BLOCK_COUNT*CUBE_SIZE;
				const int mainLeftChZ=mainChZ;
				for(int i=CHUNKS_PRELOAD_LONG_SIDE, iChInd=0; i<CHUNKS_PRELOAD_LONG_SIDE+CHUNKS_PRELOAD_SHORT_SIDE; ++i,++iChInd){
					for(int j=0; j<CHUNKS_PRELOAD_BORDER; ++j){

						if(mainUpdCycleRedFlag)
							goto emergencyExit;//экстренный выход, если приказано остановить поток

						//find current coordinates
						int currChX=mainLeftChX+j*BLOCK_COUNT*CUBE_SIZE;
						int currChZ=mainLeftChZ+iChInd*BLOCK_COUNT*CUBE_SIZE;
						if(chunksPreload[i][j] != nullptr)//если там что-то было
							delete chunksPreload[i][j];
						chunksPreload[i][j]=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
					}
				}//finished left side updating

				//update front side chunks
				const int mainFrontChX=mainChX-CHUNKS_PRELOAD_BORDER*BLOCK_COUNT*CUBE_SIZE;
				const int mainFrontChZ=mainChZ+CHUNKS_COUNT*BLOCK_COUNT*CUBE_SIZE;
				for(int i=CHUNKS_PRELOAD_LONG_SIDE+CHUNKS_PRELOAD_SHORT_SIDE,iChInd=0; i<CHUNKS_PRELOAD_LONG_SIDE*2+CHUNKS_PRELOAD_SHORT_SIDE; ++i,++iChInd){
					for(int j=0; j<CHUNKS_PRELOAD_BORDER; ++j){

						if(mainUpdCycleRedFlag)
							goto emergencyExit;//экстренный выход, если приказано остановить поток

						//find current coordinates
						int currChX=mainFrontChX+iChInd*BLOCK_COUNT*CUBE_SIZE;
						int currChZ=mainFrontChZ+j*BLOCK_COUNT*CUBE_SIZE;
						if(chunksPreload[i][j] != nullptr)//если там что-то было
							delete chunksPreload[i][j];
						chunksPreload[i][j]=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
					}
				}//finished front side updating

				//update right side chunks
				const int mainRightChX=mainChX-CHUNKS_PRELOAD_BORDER*BLOCK_COUNT*CUBE_SIZE;
				const int mainRightChZ=mainChZ;
				for(int i=CHUNKS_PRELOAD_LONG_SIDE*2+CHUNKS_PRELOAD_SHORT_SIDE, iChInd=0; i<CHUNKS_PRELOAD_ENTIRE_SIDE_LENGTH; ++i,++iChInd){
					for(int j=0; j<CHUNKS_PRELOAD_BORDER; ++j){

						if(mainUpdCycleRedFlag)
							goto emergencyExit;//экстренный выход, если приказано остановить поток

						//find current coordinates
						int currChX=mainRightChX+j*BLOCK_COUNT*CUBE_SIZE;
						int currChZ=mainRightChZ+iChInd*BLOCK_COUNT*CUBE_SIZE;
						if(chunksPreload[i][j] != nullptr)//если там что-то было
							delete chunksPreload[i][j];
						chunksPreload[i][j]=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
					}
				}//finished right side updating
				//~~END MAIN CHUNKS

				//SUB CHUNKS
				//update back side sub chunks
				const int mainBackSubChX=mainChX;
				const int mainBackSubChZ=mainChZ;
				for(int i=0; i<CHUNKS_PRELOAD_SUB_LONG_SIDE; ++i){
						if(mainUpdCycleRedFlag)
							goto emergencyExit;//экстренный выход, если приказано остановить поток

						//find current coordinates
						int currChX=mainBackSubChX+i*BLOCK_COUNT*CUBE_SIZE;
						int currChZ=mainBackSubChZ;
						if(chunksSubPreload[i] != nullptr)//если там что-то было
							delete chunksSubPreload[i];
						chunksSubPreload[i]=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
				}//finished back side sub updating

				//update left sub side chunks
				const int mainLeftSubChX=mainChX+(CHUNKS_COUNT-1)*BLOCK_COUNT*CUBE_SIZE;
				const int mainLeftSubChZ=mainChZ+BLOCK_COUNT*CUBE_SIZE;
				for(int i=CHUNKS_PRELOAD_SUB_LONG_SIDE, iChInd=0; i<CHUNKS_PRELOAD_SUB_LONG_SIDE+CHUNKS_PRELOAD_SUB_SHORT_SIDE; ++i,++iChInd){
						if(mainUpdCycleRedFlag)
							goto emergencyExit;//экстренный выход, если приказано остановить поток

						//find current coordinates
						int currChX=mainLeftSubChX;
						int currChZ=mainLeftSubChZ+iChInd*BLOCK_COUNT*CUBE_SIZE;
						if(chunksSubPreload[i] != nullptr)//если там что-то было
							delete chunksSubPreload[i];
						chunksSubPreload[i]=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
				}//finished left sub side updating

				//update front sub side chunks
				const int mainFrontSubChX=mainChX;
				const int mainFrontSubChZ=mainChZ+(CHUNKS_COUNT-1)*BLOCK_COUNT*CUBE_SIZE;
				for(int i=CHUNKS_PRELOAD_SUB_LONG_SIDE+CHUNKS_PRELOAD_SUB_SHORT_SIDE,iChInd=0; i<CHUNKS_PRELOAD_SUB_LONG_SIDE*2+CHUNKS_PRELOAD_SUB_SHORT_SIDE; ++i,++iChInd){
						if(mainUpdCycleRedFlag)
							goto emergencyExit;//экстренный выход, если приказано остановить поток

						//find current coordinates
						int currChX=mainFrontSubChX+iChInd*BLOCK_COUNT*CUBE_SIZE;
						int currChZ=mainFrontSubChZ;
						if(chunksSubPreload[i] != nullptr)//если там что-то было
							delete chunksSubPreload[i];
						chunksSubPreload[i]=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
				}//finished front sub side updating

				//update right sub side chunks
				const int mainRightSubChX=mainChX;
				const int mainRightSubChZ=mainChZ+BLOCK_COUNT*CUBE_SIZE;
				for(int i=CHUNKS_PRELOAD_SUB_LONG_SIDE*2+CHUNKS_PRELOAD_SUB_SHORT_SIDE, iChInd=0; i<CHUNKS_PRELOAD_SUB_ENTIRE_LENGTH; ++i,++iChInd){
						if(mainUpdCycleRedFlag)
							goto emergencyExit;//экстренный выход, если приказано остановить поток

						//find current coordinates
						int currChX=mainRightSubChX;
						int currChZ=mainRightSubChZ+iChInd*BLOCK_COUNT*CUBE_SIZE;
						if(chunksSubPreload[i] != nullptr)//если там что-то было
							delete chunksSubPreload[i];
						chunksSubPreload[i]=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
				}//finished right sub side updating
				//~~END SUB CHUNKS
			}
			emergencyExit:;
		}
		else{//приказано остановиться
			mainUpdCyclePaused=true;
			Sleep(100);
		}
	}
}

double GameDataPreloader::getPlEstCoordX(){
	EnterCriticalSection(&plCoordCriSec); // Заходим в к.с. (ждем входа)
	double toReturn=plEstCoordX;
	LeaveCriticalSection(&plCoordCriSec); // Выходим из к.с.
	return toReturn;
}

double GameDataPreloader::getPlEstCoordZ(){
	EnterCriticalSection(&plCoordCriSec); // Заходим в к.с. (ждем входа)
	double toReturn=plEstCoordZ;
	LeaveCriticalSection(&plCoordCriSec); // Выходим из к.с.
	return toReturn;
}