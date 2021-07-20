#include <iostream>
#include <string.h> // string manipulation
#include <chrono> // delays
#include <thread> // delays
#include <windows.h > // input https://visualstudioclient.gallerycdn.vsassets.io/extensions/visualstudioclient/microsoftvisualstudio2017installerprojects/1.0.0/1620063166533/InstallerProjects.vsix
#include <ctime> // random nummer generation

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorPos = { 32, 12 };

class MatrixCodeView;
class MatrixCodeModel;

short screenColums = 20;
short screenRows = 50;
unsigned int programUpdateStep = 300; //ms
const char programLinesMax = 20;

void PrintChar(short _x, short _y, char* _char)
{
	SetConsoleCursorPosition(hConsole, { _x, _y });
	WriteConsole(hConsole, _char, 1, NULL, NULL);
}

class MatrixCodeModel {
public:
	short column = 1;
	short row = 0;
	unsigned char speed = 0; // 1, 2, 3
	MatrixCodeModel() {};
	MatrixCodeModel(const short& _column, const char& _speed) : column(_column), speed(_speed) {}

	void Step()
	{
		row = row + speed;
	}
};

class MatrixCodeView {
private: 
	std::string trail = "abcde";
public:
	MatrixCodeModel mcm;
	MatrixCodeView() {};
	MatrixCodeView(const MatrixCodeModel& _mcm) : mcm(_mcm) {}
	std::string GenerateString()
	{
		std::string newStr = "mutable";
		for (int i = 0; i < 5; i++)
		{
			newStr[i] = '1';
		}
		return newStr;
	}
	void Draw()
	{
		for (char i = 0; i < 5; i++)
		{
			// add likely hood for 'decay' as i increases
			PrintChar(mcm.column, mcm.row + i, &trail[i]);
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
	for (char i = 0; i < programLinesMax; i++)
	{
		programLines[i] = MatrixCodeView(MatrixCodeModel(1, 1));
	}
}

void Draw()
{
	for (auto mcv : programLines)
	{
		mcv.Draw();
		mcv.mcm.Step();
	}
}

int main()
{
	Initialise();
	srand(time(NULL)); // sets the seed for RNG
	cursorPos = { (short)(abs(rand()) % 50), (short)(abs(rand()) % 30) };
	SetConsoleCursorPosition(hConsole, cursorPos);

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		//WriteConsole(hConsole, "!", 1, NULL, NULL);
		Draw();
		std::this_thread::sleep_for(std::chrono::milliseconds(programUpdateStep));
	}
	return 0;
}
