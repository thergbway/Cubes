#pragma once
#include "block.h"
#include "defines.h"

void Block::setAir(){
	type=AIR;
	transparent=true;
}

void Block::setDirt(){
	type=DIRT;
	transparent=false;
}

void Block::setGrass(){
	type=GRASS;
	transparent=false;
}

char Block::getType(){
	return type;
}

bool Block::isTransparent(){
	return transparent;
}