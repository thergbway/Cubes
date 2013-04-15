#pragma once
#include "gameMain.h"

class GameMain;

class Console
{
	//members
private:
	GameMain* gameMain;
public:
	//functions
public:
	Console(GameMain* gameMainPtr);
	void showMessage(const QString &message);
};