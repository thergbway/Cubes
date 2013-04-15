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
	double sensitivity;//���������������� ����
	bool mouseReleased;//�������� �� ����(�� ��������� ��)
	bool flyingModOn;//����� ������
public:
	//functions
public:
	Settings(GameMain* gameMainPtr);
	double getSpawnPointX();
	double getSpawnPointY();
	double getSpawnPointZ();
	double getMaxWalkSpeed();
	double getSensitivity();
	bool isFlyingModOn();
	bool isMouseReleased();
};