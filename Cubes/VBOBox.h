#pragma once
#pragma warning(disable: 4290)
#include <QGLBuffer>
#include <new>
#include <array>
#include <cstddef>//size_t
#include "gameMain.h"
#include "defines.h"
#include "gameDataPreloader.h"
#include "chunk.h"
#include "VBOBoxTransferInfo.h"

class GameMain;
class Chunk;
class VBOBoxPrebuild;
class VBOBoxTransferInfo;

class VBOBox{
private:
	GameMain* gameMain;
	QGLBuffer VBO;
	QGLBuffer NBO;//normal buffer object like VBO
	QGLBuffer TBO;//texture buffer object like VBO
	GLuint* texturesPtr;

	//���������� VBOBox
	int coorX;
	int coorZ;

	static bool alloc_map[];//��� ������������� ��������� ������
	static unsigned char pool[];//��� ������������� ��������� ������

	//���������� �����
	int pointsOfDirtToDraw;
	int pointsOfGrassTopToDraw;
	int pointsOfGrassSideToDraw;
	int pointsOfStoneToDraw;
	int pointsOfSandToDraw;
	int pointsOfWaterToDraw;
	int pointsOfLeafsToDraw;
	int pointsOfWoodToDraw;
	int pointsOfSnowToDraw;
	//offsets
	int offsetOfDirt;
	int offsetOfGrassTop;
	int offsetOfGrassSide;
	int offsetOfStone;
	int offsetOfSand;
	int offsetOfWater;
	int offsetOfLeafs;
	int offsetOfWood;
	int offsetOfSnow;
public:
	void draw();
	int getCoorX();
	int getCoorZ();
	void* operator new(size_t) throw(std::bad_alloc);
	void operator delete(void*);
	VBOBox(int chNumX,int chNumZ,GameMain* gameMain,GLuint* texuresArrayPtr,VBOBoxPrebuild* vBOBoxPrebuild = nullptr);
	~VBOBox();
};