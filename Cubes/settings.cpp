#pragma once
#include "settings.h"
#include "gameMain.h"

Settings::Settings(GameMain* gameMainPtr)
{
	gameMain=gameMainPtr;
	spawnPointX=0*CUBE_SIZE;
	spawnPointY=130*CUBE_SIZE;
	spawnPointZ=0*CUBE_SIZE;
	maxWalkSpeed=30*CUBE_SIZE;
	sensitivity=0.1;
	mouseReleased=false;
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

double Settings::getSensitivity(){
	return sensitivity;
}

bool Settings::isFlyingModOn(){
	return flyingModOn;
}

bool Settings::isMouseReleased(){
	return mouseReleased;
}