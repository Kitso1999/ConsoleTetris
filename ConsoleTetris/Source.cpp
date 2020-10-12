#include <iostream>
#include <thread>

#include <Windows.h>

using namespace std;

//Game Properties
const int BoardWidth = 10;
const int BoardHeight = 20;

//Main Game Objects*****************
const wchar_t* tetrominoes[7] = 
{
	L"..X."
	L"..X."
	L"..X."
	L"..X.",

	L".X.."
	L".XX."
	L".X.."
	L"....",

	L"...."
	L".XX."
	L".XX."
	L"....",

	L".X.."
	L".X.."
	L".XX."
	L"....",

	L"..X."
	L"..X."
	L".XX."
	L"....",

	L"..X."
	L".XX."
	L".X.."
	L"....",

	L".X.."
	L".XX."
	L"..X."
	L"...."
};


const wchar_t* assets = L" ABCDEFG=#";
//**********************************

//Game State Variables
bool bkey[4];
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

	//Game Logic
	bool GameOver = false;
	while (!GameOver)
	{

		//Game Timing
		std::this_thread::sleep_for(50ms);

		//*****************************

		//User Input
		for (int i = 0; i < 4; ++i)							//	R	L  D Z
			bkey[i] = ((0x0001) & GetAsyncKeyState((const char)"\x27\x25\x28Z"[i]));

		//*****************************
	}
	//*********************************
}