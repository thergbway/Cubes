#pragma once

class Block
{
	//members
private:
	char type;
	char brightness_top;
	char brightness_bottom;
	char brightness_left;
	char brightness_right;
	char brightness_front;
	char brightness_back;
	bool transparent;
	//functions
public:
	void setAir();
	void setDirt();
	void setGrass();
	void setStone();
	void setSand();
	char getType();
	bool isTransparent();
};