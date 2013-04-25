#include "dayNightCycleManager.h"

DayNightCycleManager::DayNightCycleManager(GameMain* _gameMain){
	gameMain=_gameMain;
	lastTick=clock();
	currentTime=DAY_NIGHT_CYCLE_TIME/2/2;//полдень
}

int DayNightCycleManager::getCurrentTime(){
	int delta=clock()-lastTick;
	lastTick=clock();
	currentTime+=delta;
	currentTime%=DAY_NIGHT_CYCLE_TIME;
	return currentTime;
}

bool DayNightCycleManager::isDay(){
	if(currentTime < DAY_NIGHT_CYCLE_TIME/2)
		return true;
	else
		return false;
}