#pragma once
#include "gameMain.h"

class GameMain;

class State
{
	//members
private:
	GameMain* gameMain;
	bool mouseReleased;
	bool flyingModOn;
	bool playerLightsOn;//фонарик и лампочка игрока
	//functions
public:
	State(GameMain* gameMainPtr);
	void switchPlayerLights();
	void switchFlyingMode();
	void setMouseReleased();
	void setMouseUnreleased();
	bool isMouseReleased();
	bool isFlyingModOn();
	bool isPlayerLightsOn();
};