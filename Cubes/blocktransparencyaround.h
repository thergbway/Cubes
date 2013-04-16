#pragma once

class BlockTransparencyAround{//прозрачность вокруг блока
public:
	bool top;
	bool down;
	bool left;
	bool right;
	bool front;
	bool back;
	BlockTransparencyAround(bool top,bool down,bool left,bool right,bool front,bool back);
};