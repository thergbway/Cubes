#pragma once
#include "chunk.h"
#include "gameMain.h"
#include "simpleArray.h"
#include "VBOBoxTransferInfo.h"

class GameMain;
class Chunk;
class VBOBoxTransferInfo;

class VBOBoxPrebuild{
	//Предзагруженная информация для VBOBox
	//Может быть использован для ускоренного создания VBOBox

	//members
public:
	VBOBoxTransferInfo* vBOBoxTransferInfo;//тут храним количества точек и размеры смещений
	//массивы вершинных и текстурных кооринат (финальные)
	SimpleArray<GLint, SIZE_OF_VERTICES_FINAL_ARRAYS> verticesFinal;
	SimpleArray<GLfloat, SIZE_OF_TEXTURES_FINAL_ARRAYS> texturesFinal;
	int coordAppChX;//координаты соответствующего чанка
	int coordAppChZ;//координаты соответствующего чанка
private:
	static bool alloc_map[];//для динамического выделения памяти
	static unsigned char pool[];//для динамического выделения памяти

	//functions
public:
	VBOBoxPrebuild(Chunk* chunk, Chunk* chBackPreloaded, Chunk* chFrontPreloaded
		,Chunk* chLeftPreloaded,Chunk* chRightPreloaded);
	~VBOBoxPrebuild();
	void* operator new(size_t) throw(std::bad_alloc);
	void operator delete(void*);
private:
};