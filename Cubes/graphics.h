#pragma once
#pragma warning(disable: 4290)
#include <QGLWidget>
#include <QGLBuffer>
#include <new>
#include <cstddef>//size_t
#include <map>
#include "vector3d.h"
#include "gameMain.h"
#include "defines.h"

class GameMain;
class VBOBox;

class Graphics : public QGLWidget
{
	Q_OBJECT
	//members
private:
	GameMain* gameMain;
	std::map<const int,VBOBox*> VBOBoxMap;
	GLuint textures[TEXTURES_COUNT];
	//functions
public:
	Graphics(GameMain* gameMainPtr,QWidget *parent = 0);
protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
private:
	void loadAllTextures();
};

class VBOBox{
private:
	GameMain* gameMain;
	QGLBuffer VBO;
	QGLBuffer TBO;//texture buffer object like VBO
	GLuint* texturesPtr;

	//координаты VBOBox
	int coorX;
	int coorZ;

	static bool alloc_map[];//для динамического выделения памяти
	static unsigned char pool[];//для динамического выделения памяти

	//количества точек
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