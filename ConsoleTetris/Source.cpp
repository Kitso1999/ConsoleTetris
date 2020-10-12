#include <iostream>

#include <Windows.h>



//Game Properties
const int BoardWidth = 10;
const int BoardHeight = 20;

//Main Game Objects*****************
const char* tetrominoes[7] = 
{
	"..X."
	"..X."
	"..X."
	"..X.",

	".X.."
	".XX."
	".X.."
	"....",

	"...."
	".XX."
	".XX."
	"....",

	".X.."
	".X.."
	".XX."
	"....",

	"..X."
	"..X."
	".XX."
	"....",

	"..X."
	".XX."
	".X.."
	"....",

	".X.."
	".XX."
	"..X."
	"...."
};
//**********************************

int Rotate(int x, int y, int r)
{
	switch (r % 4)
	{
	case 0: return y * 4 + x;
	case 1: return 3 - y + 4 * x;
	case 2: return 15 - 4 * y - x;
	case 3: return 12 + y - 4 * x;
	}
}

bool DoesPieceFit(int r)
{
	return true;
}

int main()
{
	
}