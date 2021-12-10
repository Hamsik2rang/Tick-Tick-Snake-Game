#pragma once
#include <random>

#include "Snake.h"

// Random.hpp is Header-Only file.

class Random
{
private:
	std::random_device _rd;
	std::mt19937 _mersenne;
public:
	Random()
		:_mersenne{ _rd()}
	{}

	int operator()(int min, int max)
	{
		std::uniform_int_distribution<> generator(min, max);
		return generator(_mersenne);
	}
};