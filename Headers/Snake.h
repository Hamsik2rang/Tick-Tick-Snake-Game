#pragma once
#include <cassert>

enum class Direction
{
	UP, LEFT, DOWN, RIGHT
};

Direction& operator--(Direction& d, int);
Direction& operator++(Direction& d, int);

struct Pos
{
	int x;
	int y;
};

class Snake
{
private:
	Pos _body[255];
	Pos _border;
	unsigned char _tail;
	Direction _dir;
public:
	Snake(int x, int y, Direction dir = Direction::LEFT);
	~Snake();

	void init(int x, int y, Direction dir = Direction::LEFT);

	bool move();	// if fail move(e.g, game over), return false.
	bool hasGotItem(Pos itemPos) const;
	void addTail();

	void setBorder(int x, int y);
	void setBorder(Pos pos);

	int x() const;
	int y() const;
	unsigned char length() const;
	Direction& dir();

	Pos& operator[](unsigned int index);
};