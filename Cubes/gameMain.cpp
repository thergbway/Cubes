#pragma once
#include <math.h>
#include "gameMain.h"

GameMain::GameMain(QMainWindow* _mainWin)
{
	mainWindow=_mainWin;
	settings= new Settings(this);
	info= new Info(this);
	console= new Console(this);
	state= new State(this);
	kMController=new KMController(this);
	player= new Player(this);
	graphics= new Graphics(this);
	world= new World(this);

	//setting timer
	mainGameCycleTimer=new QTimer;
	mainGameCycleTimer->setSingleShot(false);
	connect(mainGameCycleTimer, SIGNAL(timeout()), this, SLOT(makeGameStep()));
}

void GameMain::startGameOnce(){
	mainGameCycleTimer->start(0);
}

void GameMain::makeGameStep(){
	player->updatePlayerCoord();
	kMController->moveCursorToDefaultPlace();
	world->updateWorld();
	graphics->updateGL();
}