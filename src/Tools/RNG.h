#pragma once

#pragma once
#include <cstdlib>
#include <ctime>

inline void random_reset_seed()
{
	srand(static_cast<int>(time(0)));
}

inline int random_between_inclusive(int a, int b)
{
	// Ensure that a <= b for the formula to work correctly
	if(a > b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

	return a + rand() % (b - a + 1);
}

inline float random_between_inclusive(float a, float b)
{
	// Ensure that a <= b for the formula to work correctly
	if(a > b)
	{
		float temp = a;
		a = b;
		b = temp;
	}

	return a + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (b - a)));
}
