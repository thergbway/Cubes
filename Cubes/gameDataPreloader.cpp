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
		//and VBOBoxPrebuilds
		for(int i=0; i<VBOBOXPREBUILD_ENTIRE_LENGTH; ++i)
			vBOBoxPrebuild[i]=nullptr;
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
	qDebug()<<"cannot find chunk in preloads x="<<coordX<<"z="<<coordZ;
	unpauseMainUpdCycle();
	return toReturn;

}

VBOBox* GameDataPreloader::getNewVBOBoxPtr(int chNumX,int chNumZ,GameMain* gameMain,GLuint textures[TEXTURES_COUNT]){
	pauseMainUpdCycle();

	//find for preloaded VBOBox
	int coordChXNeeded=gameMain->world->getChunkCoordX(chNumX,chNumZ);
	int coordChZNeeded=gameMain->world->getChunkCoordZ(chNumX,chNumZ);
	for(int i=0; i < VBOBOXPREBUILD_ENTIRE_LENGTH; ++i){
		if(
			vBOBoxPrebuild[i] != nullptr &&
			coordChXNeeded == vBOBoxPrebuild[i]->coordAppChX &&
			coordChZNeeded == vBOBoxPrebuild[i]->coordAppChZ
			){
			//нашли нужный VBOBoxPrebuild
			VBOBoxPrebuild* currVBOBoxPrebuild=vBOBoxPrebuild[i];
			VBOBox* toReturn=new VBOBox(chNumX,chNumZ,gameMain,textures,currVBOBoxPrebuild);
			unpauseMainUpdCycle();
			return toReturn;
		}
	}
	qDebug()<<"cannot find VBOBoxPreload for VBOBox for chunk: x="<<chNumX<<"z="<<chNumZ;
	VBOBox* toReturn=new VBOBox(chNumX,chNumZ,gameMain,textures);
	unpauseMainUpdCycle();
	return toReturn;
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

	bool entireCycleIsDone=false;//цикл полностью пройден

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

			if(oldMainChX != mainChX || oldMainChZ != mainChZ || entireCycleIsDone == false){
				//если координаты главного чанка изменились или на прошлой итерации
				//цикл не был пройден до конца

				entireCycleIsDone = false;//цикл пройден не до конца

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

				//STARTING PREBUILDS VBOBOX
				//вычислим координаты главного чанка для предзагрузки графических объектов
				const int mainVBOChX=plChX-((CHUNKS_TO_DRAW-1)/2+ONE_CHUNK)*BLOCK_COUNT*CUBE_SIZE;
				const int mainVBOChZ=plChZ-((CHUNKS_TO_DRAW-1)/2+ONE_CHUNK)*BLOCK_COUNT*CUBE_SIZE;

				//update back VBOBoxPrebuilds
				const int mainBackVBOBoxPrebuildsX=mainVBOChX;
				const int mainBackVBOBoxPrebuildsZ=mainVBOChZ;
				for(int i=0; i < VBOBOXPREBUILD_LONG_SIDE; ++i){
					if(mainUpdCycleRedFlag)
						goto emergencyExit;//экстренный выход, если приказано остановить поток

					//find current coordinates
					int currChX=mainBackVBOBoxPrebuildsX+i*BLOCK_COUNT*CUBE_SIZE;
					int currChZ=mainBackVBOBoxPrebuildsZ;
					Chunk* tempCh=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
					Chunk* tempBackCh=new Chunk(world,worldLayerHolder,currChX,currChZ-BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
					Chunk* tempRightCh=new Chunk(world,worldLayerHolder,currChX+BLOCK_COUNT*CUBE_SIZE,currChZ,getNewChunkId());
					Chunk* tempFrontCh=new Chunk(world,worldLayerHolder,currChX,currChZ+BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
					Chunk* tempLeftCh=new Chunk(world,worldLayerHolder,currChX-BLOCK_COUNT*CUBE_SIZE,currChZ,getNewChunkId());

					if(vBOBoxPrebuild[i] != nullptr)
						delete vBOBoxPrebuild[i];
					vBOBoxPrebuild[i]=new VBOBoxPrebuild(tempCh,tempBackCh,tempFrontCh,tempLeftCh,tempRightCh);

					delete tempCh;
					delete tempBackCh;
					delete tempFrontCh;
					delete tempLeftCh;
					delete tempRightCh;
				}
				
				//update front VBOBoxPrebuilds
				const int mainFrontVBOBoxPrebuildsX=mainVBOChX;
				const int mainFrontVBOBoxPrebuildsZ=mainVBOChZ+(CHUNKS_TO_DRAW+ONE_CHUNK)*BLOCK_COUNT*CUBE_SIZE;
				for(int i=VBOBOXPREBUILD_LONG_SIDE+VBOBOXPREBUILD_SHORT_SIDE, offsetX=0; i < VBOBOXPREBUILD_LONG_SIDE*2+VBOBOXPREBUILD_SHORT_SIDE; ++i, ++offsetX){
					if(mainUpdCycleRedFlag)
						goto emergencyExit;//экстренный выход, если приказано остановить поток

					//find current coordinates
					int currChX=mainFrontVBOBoxPrebuildsX+offsetX*BLOCK_COUNT*CUBE_SIZE;
					int currChZ=mainFrontVBOBoxPrebuildsZ;
					Chunk* tempCh=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
					Chunk* tempBackCh=new Chunk(world,worldLayerHolder,currChX,currChZ-BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
					Chunk* tempRightCh=new Chunk(world,worldLayerHolder,currChX+BLOCK_COUNT*CUBE_SIZE,currChZ,getNewChunkId());
					Chunk* tempFrontCh=new Chunk(world,worldLayerHolder,currChX,currChZ+BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
					Chunk* tempLeftCh=new Chunk(world,worldLayerHolder,currChX-BLOCK_COUNT*CUBE_SIZE,currChZ,getNewChunkId());

					if(vBOBoxPrebuild[i] != nullptr)
						delete vBOBoxPrebuild[i];
					vBOBoxPrebuild[i]=new VBOBoxPrebuild(tempCh,tempBackCh,tempFrontCh,tempLeftCh,tempRightCh);

					delete tempCh;
					delete tempBackCh;
					delete tempFrontCh;
					delete tempLeftCh;
					delete tempRightCh;
				}
				//update left VBOBoxPrebuilds
				const int mainLeftVBOBoxPrebuildsX=mainVBOChX;
				const int mainLeftVBOBoxPrebuildsZ=mainVBOChZ+ONE_CHUNK*BLOCK_COUNT*CUBE_SIZE;
				for(int i=VBOBOXPREBUILD_LONG_SIDE*2+VBOBOXPREBUILD_SHORT_SIDE, offsetZ=0; i < VBOBOXPREBUILD_ENTIRE_LENGTH; ++i, ++offsetZ){
					if(mainUpdCycleRedFlag)
						goto emergencyExit;//экстренный выход, если приказано остановить поток

					//find current coordinates
					int currChX=mainLeftVBOBoxPrebuildsX;
					int currChZ=mainLeftVBOBoxPrebuildsZ+offsetZ*BLOCK_COUNT*CUBE_SIZE;
					Chunk* tempCh=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
					Chunk* tempBackCh=new Chunk(world,worldLayerHolder,currChX,currChZ-BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
					Chunk* tempRightCh=new Chunk(world,worldLayerHolder,currChX+BLOCK_COUNT*CUBE_SIZE,currChZ,getNewChunkId());
					Chunk* tempFrontCh=new Chunk(world,worldLayerHolder,currChX,currChZ+BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
					Chunk* tempLeftCh=new Chunk(world,worldLayerHolder,currChX-BLOCK_COUNT*CUBE_SIZE,currChZ,getNewChunkId());
					
					if(vBOBoxPrebuild[i] != nullptr)
						delete vBOBoxPrebuild[i];
					vBOBoxPrebuild[i]=new VBOBoxPrebuild(tempCh,tempBackCh,tempFrontCh,tempLeftCh,tempRightCh);

					delete tempCh;
					delete tempBackCh;
					delete tempFrontCh;
					delete tempLeftCh;
					delete tempRightCh;
				}
				//update right VBOBoxPrebuilds
				const int mainRightVBOBoxPrebuildsX=mainVBOChX+(ONE_CHUNK+CHUNKS_TO_DRAW)*BLOCK_COUNT*CUBE_SIZE;
				const int mainRightVBOBoxPrebuildsZ=mainVBOChZ+ONE_CHUNK*BLOCK_COUNT*CUBE_SIZE;
				for(int i=VBOBOXPREBUILD_LONG_SIDE, offsetX=0; i < VBOBOXPREBUILD_LONG_SIDE+VBOBOXPREBUILD_SHORT_SIDE; ++i, ++offsetX){
					if(mainUpdCycleRedFlag)
						goto emergencyExit;//экстренный выход, если приказано остановить поток

					//find current coordinates
					int currChX=mainRightVBOBoxPrebuildsX;
					int currChZ=mainRightVBOBoxPrebuildsZ+offsetX*BLOCK_COUNT*CUBE_SIZE;
					Chunk* tempCh=new Chunk(world,worldLayerHolder,currChX,currChZ,getNewChunkId());
					Chunk* tempBackCh=new Chunk(world,worldLayerHolder,currChX,currChZ-BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
					Chunk* tempRightCh=new Chunk(world,worldLayerHolder,currChX+BLOCK_COUNT*CUBE_SIZE,currChZ,getNewChunkId());
					Chunk* tempFrontCh=new Chunk(world,worldLayerHolder,currChX,currChZ+BLOCK_COUNT*CUBE_SIZE,getNewChunkId());
					Chunk* tempLeftCh=new Chunk(world,worldLayerHolder,currChX-BLOCK_COUNT*CUBE_SIZE,currChZ,getNewChunkId());
					
					if(vBOBoxPrebuild[i] != nullptr)
						delete vBOBoxPrebuild[i];
					vBOBoxPrebuild[i]=new VBOBoxPrebuild(tempCh,tempBackCh,tempFrontCh,tempLeftCh,tempRightCh);

					delete tempCh;
					delete tempBackCh;
					delete tempFrontCh;
					delete tempLeftCh;
					delete tempRightCh;
				}
				
				entireCycleIsDone = true;//цикл пройден до конца
			}
			emergencyExit:;
		}
		else{//приказано остановиться
			mainUpdCyclePaused=true;
			Sleep(3);
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