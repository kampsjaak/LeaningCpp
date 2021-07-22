#include <iostream>
// #include <string.h> // string manipulation -> using std::string instead
#include <chrono> // delays
#include <thread> // delays
#include <windows.h > // input https://visualstudioclient.gallerycdn.vsassets.io/extensions/visualstudioclient/microsoftvisualstudio2017installerprojects/1.0.0/1620063166533/InstallerProjects.vsix
#include <ctime> // random nummer generation

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPos = { 32, 12 };

class MatrixCodeView;
class MatrixCodeModel;

short screenColums = 80;
short screenRows = 24;
unsigned int programUpdateStep = 400; //ms
const char programLinesMax = 20;
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
		if(!init) row = -(rand() % 4);
	}
	void Step()
	{
		if (row > screenRows) ReRoll(false);
		else row = row + speed;
	}
};

class MatrixCodeView {
private: 
	std::string trail = "abcde";
public:
	MatrixCodeModel mcm;
	MatrixCodeView() {};
	MatrixCodeView(const MatrixCodeModel& _mcm) : mcm(_mcm) { };
	void GenerateString()
	{
		for (int i = 0; i < 5; i++)
		{
			trail[i] = GetRandomChar();
		}
	}
	void Draw()
	{
		for (char i = 0; i < trail.length(); i++)
		{
			if (mcm.row + i < 0 || mcm.row + i > screenRows) continue;
			// add chance for 'decay' as i increases
			// ---> strOut[0] = GetRandomChar();
			strOut[0] = trail.at(i);
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