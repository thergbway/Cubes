#pragma once
#include <time.h>
#include "gameMain.h"
#include "vector3d.h"

class GameMain;
class Vector3D;

class Player
{
	//members
private:
	GameMain* gameMain;
	clock_t lastFrameTime;
	double maxWalkSpeed;
	double coorX;
	double coorY;
	double coorZ;
	double horRot;
	double vertRot;
	Vector3D movingVector;//вектор движения
	//functions
public:
	Player(GameMain* gameMainPtr);
	double getCoorX();
	double getCoorY();
	double getCoorZ();
	void setCoorX(double coorX);
	void setCoorY(double coorY);
	void setCoorZ(double coorZ);
	void updatePlayerCoord();
	Vector3D getVectorOfPlayerView();
};