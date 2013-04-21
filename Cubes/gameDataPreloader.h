#pragma once
#include "chunk.h"
#include "world.h"
#include "worldlayerholder.h"
#include "VBOBox.h"

class Chunk;
class GameDataPreloader;
class World;
class VBOBox;
class GameMain;
class VBOBoxPrebuild;

//работа с запуском предзагрузчика
unsigned int __stdcall mainUpdThreadStarter(void* pParams);
extern bool mainUpdThreadStarterNeedToWait;//флаг ожидания старта потока предзагрузчика
extern GameDataPreloader* gameDataPreloaderPtr;//поток предзагрузчика будет использовать этот объект
//~конец работы с прездагрузчиком

//константы
const int CHUNKS_PRELOAD_BORDER=CHUNKS_PRELOAD_COUNT;
const int CHUNKS_PRELOAD_LONG_SIDE=CHUNKS_PRELOAD_BORDER*2+CHUNKS_COUNT;
const int CHUNKS_PRELOAD_SHORT_SIDE=CHUNKS_COUNT;
const int CHUNKS_PRELOAD_ENTIRE_SIDE_LENGTH=CHUNKS_PRELOAD_LONG_SIDE*2+CHUNKS_PRELOAD_SHORT_SIDE*2;
const int CHUNKS_PRELOAD_SUB_LONG_SIDE=CHUNKS_COUNT;//эти чанки пойдут по периметру основного поля загруженного
const int CHUNKS_PRELOAD_SUB_SHORT_SIDE=CHUNKS_COUNT-2;//эти чанки пойдут по периметру основного поля загруженного
const int CHUNKS_PRELOAD_SUB_ENTIRE_LENGTH=CHUNKS_PRELOAD_SUB_LONG_SIDE*2+CHUNKS_PRELOAD_SUB_SHORT_SIDE*2;

const int VBOBOXPREBUILD_PRELOAD_COUNT=CHUNKS_PRELOAD_ENTIRE_SIDE_LENGTH*(CHUNKS_PRELOAD_BORDER-1)+VBOBOX_PRELOAD_EXTRA_BUFFER;

class GameDataPreloader{
	//Класс предоставляет пользователям данные по чанкам, VBOBox, WorldLayerHolder.
	//Класс не имеет права взаимодействовать с внешним миром.
	//Все данные ему мы предоставляем. Класс же генерирует нужные нам данные
	//В классе работает отдельный генерирующий поток, который и генерирует сами данные
	//При изъятии данных поток останавливаем. По окончании операции снова запускаем

	//members
public:
private:
	CRITICAL_SECTION plCoordCriSec; // Критическая секция
	World* world;
	WorldLayerHolder* worldLayerHolder;
	double plEstCoordX;//примерные координаты игрока
	double plEstCoordZ;//примерные координаты игрока
	bool mainUpdCyclePaused;//флаг, контролируемый потоком
	bool mainUpdCycleRedFlag;//флаг для потока. Необходимо остановиться
	Chunk* chunksPreload[CHUNKS_PRELOAD_ENTIRE_SIDE_LENGTH][CHUNKS_PRELOAD_BORDER];
	Chunk* chunksSubPreload[CHUNKS_PRELOAD_SUB_ENTIRE_LENGTH];
	VBOBoxPrebuild* vBOBoxPrebuilds[VBOBOXPREBUILD_PRELOAD_COUNT];
	//functions
public:
	GameDataPreloader(World* world);
	~GameDataPreloader();
	Chunk* getNewChunkPtr(int coordX, int coordZ);//возвращ. готовый к использованию чанк
	VBOBox* getNewVBOBoxPtr(int chNumX,int chNumZ,GameMain* gameMain,GLuint textures[TEXTURES_COUNT]);//возвращ. готовый к использованию VBOBox
	void setPlEstCoordinates(double plEstCoordX, double plEstCoordZ);
	void mainUpdCycle();
private:
	void pauseMainUpdCycle();
	void unpauseMainUpdCycle();
	int getNewChunkId();
	double getPlEstCoordX();
	double getPlEstCoordZ();
};