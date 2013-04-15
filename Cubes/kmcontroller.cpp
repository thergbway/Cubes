#pragma once
#include <QKeyEvent>
#include <QDebug>
#include "defines.h"
#include "kmcontroller.h"

KMController::KMController(GameMain* _gameMain){
	setMouseTracking(true);
	gameMain=_gameMain;

	wPressed=false;
	aPressed=false;
	sPressed=false;
	dPressed=false;
	spacePressed=false;
	leftCtrlPressed=false;

	deltaX=0;
	deltaY=0;

	lastX=0;
	lastY=0;
}

bool KMController::eventFilter(QObject *target, QEvent *event)
{
	if(event->type() == QEvent::MouseMove){
		//обработка мыши
		int currX=QCursor::pos().x();
		int currY=QCursor::pos().y();
		deltaX+=currX-lastX;
		deltaY+=-currY+lastY;//из-за другой системы координат окна
		//переместим курсор в центр окна
		moveCursorToDefaultPlace();
		return true;
	}
	if(event->type() == QEvent::MouseButtonPress){
		//нажатие ЛКМ или ПКМ
		gameMain->state->setMouseUnreleased();
		gameMain->mainWindow->setCursor(Qt::BlankCursor);
		moveCursorToDefaultPlace();
		deltaX=0;
		deltaY=0;
		return true;
	}
	if (event->type() == QEvent::KeyPress){
		//обработка нажатия
		QKeyEvent *keyEvent = (QKeyEvent *)event;
		switch(keyEvent->key())
		{
		case Qt::Key_W:
			wPressed=true;
			return true;
		case Qt::Key_S:
			sPressed=true;
			return true;
		case Qt::Key_A:
			aPressed=true;
			return true;
		case Qt::Key_D:
			dPressed=true;
			return true;
		case Qt::Key_Space:
			spacePressed=true;
			return true;
		case Qt::Key_Control:
			leftCtrlPressed=true;
			return true;
		case Qt::Key_Escape:
			gameMain->state->setMouseReleased();
			gameMain->mainWindow->setCursor(Qt::ArrowCursor);
			return true;
		}
	}
	if(event->type()==QEvent::KeyRelease){
		//обработка отпускания клавиши
		QKeyEvent *keyEvent=(QKeyEvent *)event;
		switch(keyEvent->key())
		{
		case Qt::Key_W:
			wPressed=false;
			return true;
		case Qt::Key_S:
			sPressed=false;
			return true;
		case Qt::Key_A:
			aPressed=false;
			return true;
		case Qt::Key_D:
			dPressed=false;
			return true;
		case Qt::Key_Space:
			spacePressed=false;
			return true;
		case Qt::Key_Control:
			leftCtrlPressed=false;
			return true;
		}
	}
	return true;//события уничтожаются
}

void KMController::setLastXY(int _lastX, int _lastY){
	lastX=_lastX;
	lastY=_lastY;
}

void KMController::moveCursorToDefaultPlace(){
	//если только мышь захвачена
	if(!gameMain->state->isMouseReleased()){
		QCursor::setPos(gameMain->mainWindow->pos().x()+gameMain->mainWindow->size().width()/2,gameMain->mainWindow->pos().y()+gameMain->mainWindow->size().height()/2);
		lastX=QCursor::pos().x();
		lastY=QCursor::pos().y();
	}
}