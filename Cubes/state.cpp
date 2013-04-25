#pragma once
#include "state.h"
#include "gameMain.h"

State::State(GameMain* gameMainPtr)
{
	gameMain=gameMainPtr;
	mouseReleased=gameMain->settings->isMouseReleased();
	flyingModOn=gameMain->settings->isFlyingModOn();
	playerLightsOn=false;
}

bool State::isMouseReleased(){
	return mouseReleased;
}

void State::switchPlayerLights(){
	if(playerLightsOn)
		playerLightsOn=false;
	else
		playerLightsOn=true;
}

void State::switchFlyingMode(){
	if(flyingModOn)
		flyingModOn=false;
	else
		flyingModOn=true;
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

bool State::isPlayerLightsOn(){
	return playerLightsOn;
}