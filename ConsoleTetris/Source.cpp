#include <iostream>
#include <thread>
#include <random>
#include <vector>

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

int SpeedCounter;
int Speed;
bool ForceDown;
int NumberOfPieces;

int Score;

std::vector<int> Lines;
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
	std::uniform_int_distribution<int> di(0, 6);

	CurrentPiece = di(dre);
	CurrentX = BoardWidth / 2 - 1;
	CurrentY = 0;
	CurrentRotation = 0;
	SpeedCounter = 0;
	Speed = 20;
	NumberOfPieces = 1;
	
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
		
		//Game Timing
		std::this_thread::sleep_for(50ms);
		++SpeedCounter;
		ForceDown = (SpeedCounter == Speed);
		//*****************************

		//User Input
		for (int i = 0; i < 4; ++i)							//	R	L  D Z
			bkey[i] = ((0x0001) & GetAsyncKeyState((const char)"\x27\x25\x28Z"[i]));
			
		CurrentX += (bkey[0] && DoesPieceFit(CurrentPiece, CurrentX + 1, CurrentY, CurrentRotation));
		CurrentX -= (bkey[1] && DoesPieceFit(CurrentPiece, CurrentX - 1, CurrentY, CurrentRotation));
		CurrentY += (bkey[2] && DoesPieceFit(CurrentPiece, CurrentX, CurrentY + 1, CurrentRotation));
		CurrentRotation += (bkey[3] && DoesPieceFit(CurrentPiece, CurrentX, CurrentY, CurrentRotation + 1));
		//*****************************
		
		//Game Logic
		if (ForceDown)
		{
			//If Can ForceDown Then Do It
			if (DoesPieceFit(CurrentPiece, CurrentX, CurrentY + 1, CurrentRotation))
			{
				++CurrentY;
			}
			else
			{
				//If Not, Then:
				//Lock The Tetromino
				for (int x = 0; x < 4; ++x)
					for (int y = 0; y < 4; ++y)
						if (tetrominoes[CurrentPiece][Rotate(x, y, CurrentRotation)] == L'X')
							GameBoard[(CurrentY + y) * BoardWidth + (CurrentX + x)] = CurrentPiece + 1;

				//Test If There's Lines
				for (int y = CurrentY; y < CurrentY + 4; ++y)
					if (y < BoardHeight - 1 && y > 0)
					{
						bool IsLine = true;
						for (int x = 1; x < BoardWidth - 1; ++x)
						{
							if (GameBoard[y * BoardWidth + x] == 0)
							{
								IsLine = false;
								break;
							}
						}
						if (IsLine)
						{
							for (int x = 1; x < BoardWidth - 1; ++x)
								GameBoard[y * BoardWidth + x] = 8;
							Lines.push_back(y);
						}
					}
							



				//Get a new Tetromino
				CurrentPiece = di(dre);
				CurrentX = BoardWidth / 2 - 1;
				CurrentY = 0;
				CurrentRotation = 0;

				++NumberOfPieces;
				if (NumberOfPieces % 10 == 0 && NumberOfPieces >= 10) //Ocassionally increase the game speed
					--Speed;

				//Scoring 
				Score += 15;
				if (!Lines.empty())
					Score += (1 << Lines.size()) * 100;
				//**************************

				if (!DoesPieceFit(CurrentPiece, CurrentX, CurrentY, CurrentRotation))
					break;
			}
			
			SpeedCounter = 0;
		}

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
		
		if (!Lines.empty())
		{
			WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
			std::this_thread::sleep_for(400ms);

			for(auto& v : Lines)
				for (int x = 1; x < BoardWidth - 1; ++x)
				{
					for (int y = v; y > 0; --y)
						GameBoard[y * BoardWidth + x] = GameBoard[(y - 1) * BoardWidth + x];
					GameBoard[x] = 0;
				}
			Lines.clear();
		}

		//Draw Output
		WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
		swprintf_s(&screen[6 * ScreenWidth + (2 + BoardWidth) + 6], 16, L"Score: %8d", Score);
		//*****************************
		//*********************************
	}

	CloseHandle(hConsole);
	delete GameBoard;
	GameBoard = nullptr;
	std::cout << "Game Over! Final Score: " << Score << std::endl;
	system("pause");
 }