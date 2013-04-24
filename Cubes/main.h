#include <QApplication>
#include <QMainWindow>
#include <QSplashScreen>
#include <crtdbg.h>
#include <process.h>
#include "gameMain.h"
#include "gameDataPreloader.h"
#include "defines.h"

#pragma comment(linker, "/stack:200000000")//необходимо увеличить стек
//#pragma comment(linker, "/stack:300000000")//необходимо увеличить стек

int main(int argc,char* argv[])
{
	QApplication app(argc,argv);

	mainUpdThreadStarterNeedToWait=true;//запрещаем потоку стартовать
	gameDataPreloaderPtr=nullptr;
	_beginthreadex(NULL, 0, mainUpdThreadStarter,NULL,0,NULL);//запускаем поток- он будет ждать старта

	QSplashScreen* splashScreen=new QSplashScreen;
	QString splashScreenAdress;
	splashScreenAdress=GAME_DIRECTORY;
	splashScreenAdress.append(LOADING_SPLASH_SCREEN_NAME);
	splashScreen->setPixmap(QPixmap(splashScreenAdress));
	splashScreen->show();

	QMainWindow* mainWindow= new QMainWindow;
	GameMain* gameMain= new GameMain(mainWindow);
	mainWindow->setCentralWidget(gameMain->graphics);
	mainWindow->setMouseTracking(true);//постоянный опрос мыши
	mainWindow->setCursor(Qt::BlankCursor);//уберем курсор
	mainWindow->installEventFilter(gameMain->kMController);
	mainWindow->show();
	splashScreen->finish(mainWindow);
	delete splashScreen;

	qDebug()<<"NOTE: random has only 2 iterations";

	//starting makeGameCycle
	gameMain->startGameOnce();

	return app.exec();
}