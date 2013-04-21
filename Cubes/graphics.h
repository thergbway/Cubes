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
#include "VBOBox.h"

class GameMain;
class VBOBox;
class GameDataPreloader;

class Graphics : public QGLWidget
{
	Q_OBJECT
	//members
private:
	GameMain* gameMain;
	GameDataPreloader* gameDataPreloader;
	std::map<const int,VBOBox*> VBOBoxMap;
	GLuint textures[TEXTURES_COUNT];
	//functions
public:
	Graphics(GameMain* gameMainPtr,GameDataPreloader* gameDataPreloader,QWidget *parent = 0);
protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
private:
	void loadAllTextures();
};