#pragma once
#include "gameMain.h"

class GameMain;

class Settings
{
	//members
private:
	GameMain* gameMain;
	double spawnPointX, spawnPointY, spawnPointZ;
	double maxWalkSpeed;//per second
	double maxRunSpeed;//per second
	double sensitivity;//чувствительность мыши
	bool mouseReleased;//отпущена ли мышь(не захвачена ли)
	bool flyingModOn;//режим полета
public:
	//functions
public:
	Settings(GameMain* gameMainPtr);
	double getSpawnPointX();
	double getSpawnPointY();
	double getSpawnPointZ();
	double getMaxWalkSpeed();
	double getMaxRunSpeed();
	double getSensitivity();
	bool isFlyingModOn();
	bool isMouseReleased();
};