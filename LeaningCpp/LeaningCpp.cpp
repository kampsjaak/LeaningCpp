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

short screenColums = 45;
short screenRows = 30;
unsigned int programUpdateStep = 300; //ms
const char programLinesMax = 20;
char strOut[1] = { 'S' };
char* ptrOut = strOut;

/*
 - class properties are global/shared, this fucks up the OOP logic
*/
char GetRandomChar();

void PrintChar(short _x, short _y, char* _char)
{
	SetConsoleCursorPosition(hConsole, { _x, _y });
	WriteConsole(hConsole, _char, 1, NULL, NULL);
}

class MatrixCodeModel {
public:
	short column = 1;
	short row = 0; 
	unsigned char speed = 1; // 1, 2, 3
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
	MatrixCodeModel* ptrMcm; // is this required? just guessing at this point. need better debugging...
	MatrixCodeView() {};
	MatrixCodeView(const MatrixCodeModel& _mcm) : mcm(_mcm) { ptrMcm = &mcm; }
	void GenerateString()
	{
		for (int i = 0; i < 5; i++)
		{
			trail[i] = GetRandomChar();
		}
	}
	void Draw()
	{
		for (char i = 0; i < 5; i++)
		{
			//if (mcm.row + i <= 0) return;

			// add chance for 'decay' as i increases
			strOut[0] = GetRandomChar(); 
			//strOut[0] = trail.at(i);
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
	for (char i = 0; i < programLinesMax; i++)
	{
		programLines[i] = MatrixCodeView(MatrixCodeModel( rand() % 50, rand() % 2 + 1 ));
	}
}

void Draw()
{
	system("cls");
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
		Draw();
		std::this_thread::sleep_for(std::chrono::milliseconds(programUpdateStep));
	}
	return 0;
}
