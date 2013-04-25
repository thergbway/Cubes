#pragma once
#include <ctime>
#include "gameMain.h"
#include "defines.h"

class DayNightCycleManager{
	//�����, �������������� ���� ��� ��� ����

	//members
private:
	GameMain* gameMain;
	long long lastTick;
	int currentTime;
public:
	//functions
private:

public:
	DayNightCycleManager(GameMain* gameMain);
	int getCurrentTime();
};