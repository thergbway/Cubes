#pragma once
#include <QGLWidget>
#include <QGLBuffer>
#include <map>
#include "vector3d.h"
#include "gameMain.h"

class GameMain;
class VBOBox;

class Graphics : public QGLWidget
{
	Q_OBJECT
	//members
private:
	GameMain* gameMain;
	std::map<const int,VBOBox*> VBOBoxMap;
	//functions
public:
	Graphics(GameMain* gameMainPtr,QWidget *parent = 0);
protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
private:
};

class VBOBox{
private:
	unsigned int colorFORTEST;
	GameMain* gameMain;
	QGLBuffer* VBO;
	int pointsToDraw;
public:
	void draw();
	VBOBox(int chNumX,int chNumZ,GameMain* gameMain);
	~VBOBox();
};