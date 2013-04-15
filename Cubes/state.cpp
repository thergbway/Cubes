#pragma once
#include "state.h"
#include "gameMain.h"

State::State(GameMain* gameMainPtr)
{
	gameMain=gameMainPtr;
	mouseReleased=gameMain->settings->isMouseReleased();
	flyingModOn=gameMain->settings->isFlyingModOn();
}

bool State::isMouseReleased(){
	return mouseReleased;
}

void State::setMouseReleased(){
	mouseReleased=true;
}

void State::setMouseUnreleased(){
	mouseReleased=false;
}

bool State::isFlyingModOn(){
	return flyingModOn;
}