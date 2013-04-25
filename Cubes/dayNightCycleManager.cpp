#include "dayNightCycleManager.h"

DayNightCycleManager::DayNightCycleManager(GameMain* _gameMain){
	gameMain=_gameMain;
	lastTick=clock();
	currentTime=STARTING_TIME;
}

int DayNightCycleManager::getCurrentTime(){
	int delta=clock()-lastTick;
	lastTick=clock();
	currentTime+=delta;
	currentTime%=DAY_NIGHT_CYCLE_TIME;
	return currentTime;
}