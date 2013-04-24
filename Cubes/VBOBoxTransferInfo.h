#pragma once
#include "chunk.h"
#include "gameMain.h"
#include "simpleArray.h"

class Chunk;
class GameMain;

//класс, необходимый для передачи параметров в вызвавшую функцию
class VBOBoxTransferInfo{
public:
	//points count
	int pointsOfDirtToDraw;
	int pointsOfGrassTopToDraw;
	int pointsOfGrassSideToDraw;
	int pointsOfStoneToDraw;
	int pointsOfSandToDraw;
	//offsets
	int offsetOfDirt;
	int offsetOfGrassTop;
	int offsetOfGrassSide;
	int offsetOfStone;
	int offsetOfSand;

	VBOBoxTransferInfo& operator=(const VBOBoxTransferInfo&);
};

VBOBoxTransferInfo updateGraphicsArraysForChunk(SimpleArray<GLint,SIZE_OF_VERTICES_FINAL_ARRAYS> &verticesFinal,SimpleArray<GLfloat, SIZE_OF_TEXTURES_FINAL_ARRAYS> &texturesFinal,
	GameMain* gameMain,Chunk* chunk,int chNumX,int chNumZ,bool isPreloading,
	Chunk* chBackPreloaded = nullptr,Chunk* chFrontPreloaded = nullptr,Chunk* chLeftPreloaded = nullptr,Chunk* chRightPreloaded = nullptr);//функция, заносящая графическую геометрию в передаваемые векторы и отдающая информацию дополнительную