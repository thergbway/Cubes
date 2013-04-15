#pragma once
#include "block.h"
#include "defines.h"

void Block::setAir(){
	type=AIR;
}

void Block::setDirt(){
	type=DIRT;
}

char Block::getType(){
	return type;
}