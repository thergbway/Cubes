#include <QApplication>
#include <QMainWindow>
#include <QSplashScreen>
#include <crtdbg.h>
#include "gameMain.h"
#include "defines.h"

int main(int argc,char* argv[])
{
	QApplication app(argc,argv);

	QSplashScreen* splashScreen=new QSplashScreen;
	QString splashScreenAdress;
	splashScreenAdress=GAME_DIRECTORY;
	splashScreenAdress.append(LOADING_SPLASH_SCREEN_NAME);
	splashScreen->setPixmap(QPixmap(splashScreenAdress));
	splashScreen->show();
	//Sleep(10000);

	QMainWindow* mainWindow= new QMainWindow;
	GameMain* gameMain= new GameMain(mainWindow);
	mainWindow->setCentralWidget(gameMain->graphics);
	mainWindow->setMouseTracking(true);//постоянный опрос мыши
	mainWindow->setCursor(Qt::BlankCursor);//уберем курсор
	mainWindow->installEventFilter(gameMain->kMController);
	mainWindow->show();
	splashScreen->finish(mainWindow);
	delete splashScreen;

	//starting makeGameCycle
	gameMain->startGameOnce();

	return app.exec();
}