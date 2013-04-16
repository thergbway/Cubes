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

void Block::setStone(){
	type=STONE;
	transparent=false;
}

void Block::setSand(){
	type=SAND;
	transparent=false;
}

char Block::getType(){
	return type;
}

bool Block::isTransparent(){
	return transparent;
}