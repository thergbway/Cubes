#pragma once
#include <math.h>
#include <stdlib.h>
#include <QDebug>
#include "gameMain.h"

GameMain::GameMain(QMainWindow* _mainWin)
{
	mainWindow=_mainWin;
	dayNightCycleManager= new DayNightCycleManager(this);
	settings= new Settings(this);
	info= new Info(this);
	console= new Console(this);
	state= new State(this);
	kMController=new KMController(this);
	player= new Player(this);
	world= new World(this);
	GameDataPreloader* gameDataPreloaderPtr=world->getGameDataPreloaderPtr();
	graphics= new Graphics(this,gameDataPreloaderPtr);

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