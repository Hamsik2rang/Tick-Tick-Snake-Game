#pragma once

#ifdef _DEBUG
#include <iostream>
#endif

#include "Random.hpp"
#include "Snake.h"

class ItemGenerator
{
private:
	Random random;
	int _x;
	int _y;
	int _pixelSize;
public:
	ItemGenerator();
	ItemGenerator(int maxX, int maxY);

	void setBorder(int x, int y);

	Pos getItemPos();
};