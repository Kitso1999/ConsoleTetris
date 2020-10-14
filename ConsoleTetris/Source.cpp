#include <iostream>
#include <thread>
#include <random>

#include <Windows.h>

using namespace std;

//Game Properties
const int BoardWidth = 10;
const int BoardHeight = 18;

int ScreenWidth = 80;
int ScreenHeight = 30;

//Main Game Objects*****************
unsigned char* GameBoard = nullptr;

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

	L".XX."
	L".XX."
	L"...."
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

int CurrentPiece;
int CurrentX;
int CurrentY;
int CurrentRotation;
//**********************************

int Rotate(int x, int y, int r)
{
	switch (r % 4)
	{
	case 0: return y * 4 + x;
	case 1: return 12 + y - 4 * x;
	case 2: return 15 - 4 * y - x;
	case 3: return 3 - y + 4 * x;
	}
}

bool DoesPieceFit(int nTetromino, int xPos, int yPos, int Rotation)
{
	for (int px = 0; px < 4; ++px)
		for (int py = 0; py < 4; ++py)
		{
			int pi = Rotate(px, py, Rotation);

			int bi = (yPos + py) * BoardWidth + (xPos + px);
			if (xPos + px >= 0 && xPos < BoardWidth && yPos >= 0 && yPos < BoardHeight)
				if(tetrominoes[nTetromino][pi] == L'X' && GameBoard[bi] != 0)
					return false;
		}
				
	return true;
}

int main()
{
	//Game Variables Initialization
	std::default_random_engine dre(time(NULL));
	std::uniform_int_distribution<short> di(1, 7);

	CurrentPiece = di(dre);
	CurrentX = BoardWidth / 2 - 1;
	CurrentY = 0;
	CurrentRotation = 0;

	GameBoard = new unsigned char [BoardWidth * BoardHeight];
	for (int x = 0; x < BoardWidth; ++x)
		for (int y = 0; y < BoardHeight; ++y)
			GameBoard[y * BoardWidth + x] = (x == 0 || x == BoardWidth - 1 || y == BoardHeight - 1) ? 9 : 0;
	//***************************************
	//Screen Output Assets
	wchar_t* screen = new wchar_t[ScreenWidth * ScreenHeight];
	for (int i = 0; i < ScreenWidth * ScreenHeight; ++i) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	//********************
	bool GameOver = false;
	while (!GameOver)
	{
		//Game Logic
		//Game Timing
		std::this_thread::sleep_for(50ms);

		//*****************************

		//User Input
		for (int i = 0; i < 4; ++i)							//	R	L  D Z
			bkey[i] = ((0x0001) & GetAsyncKeyState((const char)"\x27\x25\x28Z"[i]));
			
		CurrentX += (bkey[0] && DoesPieceFit(CurrentPiece, CurrentX + 1, CurrentY, CurrentRotation));
		CurrentX -= (bkey[1] && DoesPieceFit(CurrentPiece, CurrentX - 1, CurrentY, CurrentRotation));
		CurrentY += (bkey[2] && DoesPieceFit(CurrentPiece, CurrentX, CurrentY + 1, CurrentRotation));
		CurrentRotation += (bkey[3] && DoesPieceFit(CurrentPiece, CurrentX, CurrentY, CurrentRotation + 1));
		//*****************************
		
		//Screen Output
		//Compose Frame
		for (int x = 0; x < BoardWidth; ++x)
			for (int y = 0; y < BoardHeight; ++y)
				screen[(y + 2) * ScreenWidth + (x + 2)] = L" ABCDEFG=#"[GameBoard[y * BoardWidth + x]];
		
		for (int x = 0; x < 4; ++x)
			for (int y = 0; y < 4; ++y)
				if (tetrominoes[CurrentPiece][Rotate(x, y, CurrentRotation)] == L'X')
					screen[(CurrentY + y + 2) * ScreenWidth + (CurrentX + x + 2)] = CurrentPiece + 65;
		//*****************************
		
		

		//Draw Output
		WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
		//*****************************
		//*********************************
	}
	
}