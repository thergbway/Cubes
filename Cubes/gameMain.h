#pragma once
#include <QTimer>
#include <QMainWindow>
#include "graphics.h"
#include "console.h"
#include "state.h"
#include "world.h"
#include "player.h"
#include "info.h"
#include "settings.h"
#include "kmcontroller.h"
#include "dayNightCycleManager.h"

class Graphics;
class Console;
class State;
class World;
class Player;
class Info;
class Settings;
class KMController;
class DayNightCycleManager;

class GameMain: public QObject
{
	Q_OBJECT
	//members
public:
	QMainWindow* mainWindow;
	QTimer* mainGameCycleTimer;
	DayNightCycleManager* dayNightCycleManager;
	Graphics* graphics;
	Console* console;
	State* state;
	World* world;
	Player* player;
	Info* info;
	Settings* settings;
	KMController* kMController;//принимает все нажатия клавиш
	//functions
public:
	GameMain(QMainWindow* mainWindow);
	void startGameOnce();
public slots:
	void makeGameStep();
};