#pragma once
#include <QWidget>
#include <QDebug>
#include "gameMain.h"

class KMController:public QWidget
{
	Q_OBJECT
private:
	GameMain* gameMain;
	int lastX;
	int lastY;
public:
	bool wPressed;
	bool aPressed;
	bool sPressed;
	bool dPressed;
	bool spacePressed;
	bool leftShiftPressed;
	bool leftCtrlPressed;

	int deltaX;
	int deltaY;

	KMController(GameMain* gameMain);
	void setLastXY(int lastX, int lastY);
	void moveCursorToDefaultPlace();
protected:
	bool eventFilter(QObject *target, QEvent *event);
};