#pragma once
#include <QDebug>
#include <QString>
#include "console.h"
#include "gameMain.h"

Console::Console(GameMain* gameMainPtr)
{
	gameMain= gameMainPtr;
}
void Console::showMessage(const QString &message)
{
	qDebug(message.toStdString().data());
}