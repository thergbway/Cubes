#pragma once
#include "settings.h"
#include "gameMain.h"

Settings::Settings(GameMain* gameMainPtr)
{
	gameMain=gameMainPtr;
	spawnPointX=2048*CUBE_SIZE;
	spawnPointY=110*CUBE_SIZE;
	spawnPointZ=2048*CUBE_SIZE;
	maxWalkSpeed=5*CUBE_SIZE;
	maxRunSpeed=15*CUBE_SIZE;
	sensitivity=0.1;
	mouseReleased=true;
	flyingModOn=true;
}

double Settings::getSpawnPointX(){
	return spawnPointX;
}

double Settings::getSpawnPointY(){
	return spawnPointY;
}

double Settings::getSpawnPointZ(){
	return spawnPointZ;
}

double Settings::getMaxWalkSpeed(){
	return maxWalkSpeed;
}

double Settings::getMaxRunSpeed(){
	return maxRunSpeed;
}

double Settings::getSensitivity(){
	return sensitivity;
}

bool Settings::isFlyingModOn(){
	return flyingModOn;
}

bool Settings::isMouseReleased(){
	return mouseReleased;
}