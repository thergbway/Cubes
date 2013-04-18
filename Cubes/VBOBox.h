#pragma once
#pragma warning(disable: 4290)
#include <QGLBuffer>
#include <new>
#include <cstddef>//size_t
#include "gameMain.h"
#include "defines.h"
#include "gameDataPreloader.h"
#include "chunk.h"

class GameMain;
class Chunk;
class VBOBoxPrebuild;
class VBOBoxTransferInfo;

VBOBoxTransferInfo updateGraphicsArraysForChunk(std::vector<GLint> &verticesFinal,std::vector<GLfloat> &texturesFinal,GameMain* gameMain,Chunk* chunk,bool isPreloading,int chNumX,int chNumZ);

class VBOBox{
private:
	GameMain* gameMain;
	QGLBuffer VBO;
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
	//offsets
	int offsetOfDirt;
	int offsetOfGrassTop;
	int offsetOfGrassSide;
	int offsetOfStone;
	int offsetOfSand;
public:
	void draw();
	int getCoorX();
	int getCoorZ();
	void* operator new(size_t) throw(std::bad_alloc);
	void operator delete(void*);
	VBOBox(int chNumX,int chNumZ,GameMain* gameMain,GLuint* texuresArrayPtr);
	~VBOBox();
};

class VBOBoxPrebuild{
	//��������������� ���������� ��� VBOBox
	//����� ���� ����������� ��� ����������� �������� VBOBox

	//members
public:
private:
	static bool alloc_map[];//��� ������������� ��������� ������
	static unsigned char pool[];//��� ������������� ��������� ������

	int chunkId;//id of appropriate chunk
	//���������� �����
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
	//������� ��������� � ���������� �������� (���������)
	std::vector<GLint> verticesFinal;
	std::vector<GLfloat> texturesFinal;

	//functions
public:
	void* operator new(size_t) throw(std::bad_alloc);
	void operator delete(void*);
private:
};

//�����, ����������� ��� �������� ���������� � ��������� �������
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
};