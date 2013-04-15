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
	//functions
public:
	State(GameMain* gameMainPtr);
	void setMouseReleased();
	void setMouseUnreleased();
	bool isMouseReleased();
	bool isFlyingModOn();
};