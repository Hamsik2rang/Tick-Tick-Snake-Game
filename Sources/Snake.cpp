#include "../Headers/Snake.h"


Direction& operator--(Direction& d, int)
{
	switch (d)
	{
	case Direction::UP:
		d = Direction::LEFT;
		break;
	case Direction::LEFT:
		d = Direction::DOWN;
		break;
	case Direction::DOWN:
		d = Direction::RIGHT;
		break;
	case Direction::RIGHT:
		d = Direction::UP;
		break;
	default:
		break;
	}

	return d;
}

Direction& operator++(Direction& d, int)
{
	switch (d)
	{
	case Direction::UP:
		d = Direction::RIGHT;
		break;
	case Direction::LEFT:
		d = Direction::UP;
		break;
	case Direction::DOWN:
		d = Direction::LEFT;
		break;
	case Direction::RIGHT:
		d = Direction::DOWN;
	default:
		break;
	}

	return d;
}

Snake::Snake(int x, int y, Direction dir)
	:_dir(dir), _tail(1), _body{ {0,0 } }
{
	_body[0].x = x;
	_body[0].y = y;
}

Snake::~Snake()
{}

void Snake::init(int x, int y, Direction dir)
{
	_tail = 1;
	_body[0].x = x;
	_body[0].y = y;
	_dir = dir;
}

bool Snake::move()
{
	static int dx[4]{ 0, -20, 0, 20 };
	static int dy[4]{ -20, 0, 20, 0 };

	auto nx = _body[0].x + dx[static_cast<int>(_dir)];
	auto ny = _body[0].y + dy[static_cast<int>(_dir)];

	if (nx < 0 || nx > _border.x || ny < 0 || ny > _border.y)
	{
		return false;
	}

	for (int i = 2; i < _tail; i++)
	{
		if (_body[0].x == _body[i].x && _body[0].y == _body[i].y)
		{
			return false;
		}
	}

	// move body to next position.
	// tails
	for (int i = _tail; i > 0; i--)
	{
		_body[i] = _body[i - 1];
	}
	// head
	_body[0].x = nx;
	_body[0].y = ny;

	return true;
}

bool Snake::hasGotItem(Pos itemPos) const
{
	return (_body[0].x == itemPos.x && _body[0].y == itemPos.y);
}

void Snake::addTail()
{
	_tail++;
}

void Snake::setBorder(int x, int y)
{
	_border.x = x;
	_border.y = y;
}

void Snake::setBorder(Pos pos)
{
	_border = pos;
}

int Snake::x() const
{
	return _body[0].x;
}

int Snake::y() const
{
	return _body[0].y;
}

unsigned char Snake::length() const
{
	return _tail;
}

Direction& Snake::dir()
{
	return _dir;
}

Pos& Snake::operator[](unsigned int index)
{
	assert(index >= 0 && index < _tail);
	return _body[index];
}
