#include <iostream>
// #include <string.h> // string manipulation -> using std::string instead
#include <chrono> // delays
#include <thread> // delays
#include <windows.h > // input https://visualstudioclient.gallerycdn.vsassets.io/extensions/visualstudioclient/microsoftvisualstudio2017installerprojects/1.0.0/1620063166533/InstallerProjects.vsix
#include <ctime> // random nummer generation
#include <WinCon.h> // console colours

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPos = { 0, 0 };

class MatrixCodeView;
class MatrixCodeModel;

short screenColums = 119;
short screenRows = 29;
unsigned int programUpdateStep = 300; //ms
const char programLinesMax = 25;
const char lineLengthMin = 5;
const char lineLengthMax = 10;
char strOut[1] = { 'S' };
char* ptrOut = strOut;

char GetRandomChar();

void PrintChar(short _x, short _y, char* _char)
{
	SetConsoleCursorPosition(hConsole, { _x, _y });
	WriteConsole(hConsole, _char, 1, NULL, NULL);
}

class MatrixCodeModel {
public:
	short column;
	short row = rand() % screenRows;
	unsigned char speed = 1; // 1, 2, 3, ...
	MatrixCodeModel() { ReRoll(true); }
	void ReRoll(bool init)
	{
		column = rand() % screenColums;
		speed = (rand() % 3) + 1;
		if(!init) row = -(lineLengthMax + (rand() % lineLengthMax));
	}
	void Step()
	{
		if (row > screenRows) ReRoll(false);
		else row = row + speed;
	}
};

class MatrixCodeView {
private: 
	std::string trail = "abcdefghijklm";
public:
	MatrixCodeModel mcm;
	MatrixCodeView() {};
	MatrixCodeView(const MatrixCodeModel& _mcm) : mcm(_mcm) { };
	void GenerateString()
	{
		char strLength = trail.length();
		for (int i = 0; i < strLength; i++)
		{
			trail.at(i) = GetRandomChar();
		}
	}
	void Draw()
	{
		//GenerateString();
		char strLength = trail.length();
		char half = round(strLength / 2);

		for (char i = 0; i < strLength; i++)
		{
			if (mcm.row + i < 0 || mcm.row + i > screenRows) continue;
			
			strOut[0] = trail.at(i);
			if(i == strLength - 1) SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			else if(i == strLength - 2) SetConsoleTextAttribute(hConsole, CYAN);
			else if (i < half) SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
			else SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			
			PrintChar(mcm.column, mcm.row + i, ptrOut);
		}
	}
};

MatrixCodeView programLines[programLinesMax];

char GetRandomChar()
{
	return rand() % 255;
}

void Initialise()
{
	srand(time(NULL)); // sets the seed for RNG

	for (char i = 0; i < programLinesMax; i++)
	{
		programLines[i] = MatrixCodeView(MatrixCodeModel());
	}
}

void Draw()
{
	system("cls");
	for (MatrixCodeView &mcv : programLines)
	{
		mcv.Draw();
		mcv.mcm.Step();
	}
	SetConsoleCursorPosition(hConsole, cursorPos);
}

int main()
{
	Initialise();
	
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		Draw();
		std::this_thread::sleep_for(std::chrono::milliseconds(programUpdateStep));
	}
	system("cls");
	return 0;
}