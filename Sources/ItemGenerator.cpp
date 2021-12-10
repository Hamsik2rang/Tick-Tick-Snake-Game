#include "../Headers/ItemGenerator.h"

ItemGenerator::ItemGenerator()
	:_x{ 620 }, _y{ 400 }, _pixelSize{ 20 }
{}

ItemGenerator::ItemGenerator(int maxX, int maxY)
	: _x{ maxX }, _y{ maxY }, _pixelSize{ 20 }
{}

void ItemGenerator::setBorder(int x, int y)
{
	_x = x;
	_y = y;
}

Pos ItemGenerator::getItemPos()
{
	Pos next;
	next.x = random(0, _x / _pixelSize) * _pixelSize;
	next.y = random(0, _y / _pixelSize) * _pixelSize;

	return next;
}