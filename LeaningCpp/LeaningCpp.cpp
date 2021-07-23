#include <iostream>
// #include <string.h> // string manipulation -> using std::string instead
#include <chrono> // delays
#include <thread> // delays
#include <windows.h > // input https://visualstudioclient.gallerycdn.vsassets.io/extensions/visualstudioclient/microsoftvisualstudio2017installerprojects/1.0.0/1620063166533/InstallerProjects.vsix
#include <ctime> // random nummer generation

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
class MatrixCodeStaticView;
class MatrixCodeStaticModel;

const short screenColums = 119;
const short screenRows = 29;
std::string clr = "";
int randomArray[10];

unsigned char tick = 0;
unsigned int programUpdateStep = 100; //ms
const char programLinesMax = 20;
const char staticLinesMax = 35;
const char lineLengthMin = 5;
const char lineLengthMax = 10;
char strOut[1] = { 'S' };
char* ptrOut = strOut;

std::string GetRandomChar();

unsigned char cycle = 0;
int temp;
int RandomInt()
{
	return rand();
	
	temp = randomArray[cycle % 10];
	cycle++;
	
	switch (cycle % 1)
	{
	case 1:
		return temp >> 1;
	case 0:
		return temp;
	}
	return temp;
}

void PrintChar(short _x, short _y, char* _char)
{
	if (_x < 0 || _x > screenColums) return;
	if (_y < 0 || _y > screenRows) return;
	SetConsoleCursorPosition(hConsole, { _x, _y });
	WriteConsole(hConsole, _char, 1, NULL, NULL);
}

std::string GenerateString(std::string& _str)
{
	std::string newStr = "";
	for (unsigned int i = 0; i < _str.length(); i++)
	{
		newStr.append(GetRandomChar());
	}
	return newStr;
}

class MatrixCodeModel {
public:
	short column;
	short row = RandomInt() % screenRows;
	MatrixCodeModel() { ReRoll(true); }
	void ReRoll(bool init)
	{
		int random = RandomInt();
		column = random % screenColums;
		if(!init) row = -(lineLengthMax + (random % lineLengthMax));
	}
	void Step()
	{
		if (row > screenRows) ReRoll(false);
		else row++;
	}
};

const char* lols = " ";

class MatrixCodeView {
private: 
	std::string trailA = "abcdefghijklm";
	std::string trailB = "abcdefghijklm";
	std::string trailC = "abcdefghijklm";
	std::string trailD = "abcdefghijklm";
public:
	MatrixCodeModel mcm;
	MatrixCodeView() {};
	MatrixCodeView(const MatrixCodeModel& _mcm) : mcm(_mcm) { Initialise();  };
	
	void Initialise()
	{	// pre-generate strings, much fasters than per each character
		trailA = GenerateString(trailA);
		trailB = GenerateString(trailB);
		trailC = GenerateString(trailC);
		trailD = GenerateString(trailD);
	}
	void Draw()
	{
		char n = tick % 4;
		std::string local[4] = { trailA, trailB, trailC, trailD };
		char strLength = local[n].length();
		char half = round(strLength / 2);
		
		std::string empty = " ";
		strOut[0] = empty.at(0);
		PrintChar(mcm.column, mcm.row - 1, ptrOut);

		for (char i = strLength - 1; i >= 0; i--)
		{
			if (mcm.row + i < 0 || mcm.row + i > screenRows) continue;
			
			strOut[0] = local[n].at(i);
			if(i == strLength - 1) SetConsoleTextAttribute(hConsole, WHITE);
			else if(i == strLength - 2) SetConsoleTextAttribute(hConsole, CYAN);
			//else if (i < half) SetConsoleTextAttribute(hConsole, GREEN);
			else SetConsoleTextAttribute(hConsole, LIGHTGREEN);
			
			PrintChar(mcm.column, mcm.row + i, ptrOut);
		}
	}
};

class MatrixCodeStaticModel {
public:
	short column;
	short row = RandomInt() % screenRows;
	MatrixCodeStaticModel() { ReRoll(true); }
	void ReRoll(bool init)
	{
		column = RandomInt() % screenColums;
		if (!init) row = -(lineLengthMax + (RandomInt() % lineLengthMax));
	}
	void Step()
	{
		// do something	
	}
};

// static non moving text (should be mostly mulitple columns?)
class MatrixCodeStaticView {
private:
	std::string trail = "abcdefghijklm";
public:
	MatrixCodeStaticModel mcsm;
	MatrixCodeStaticView() {};
	MatrixCodeStaticView(const MatrixCodeStaticModel& _mcsm) : mcsm(_mcsm) { Initialise();  };
	void Initialise()
	{
		trail = GenerateString(trail);
	}
	void Draw()
	{
		for (char i = trail.length() - 1; i >= 0; i--)
		{
			strOut[0] = trail.at(i);
			SetConsoleTextAttribute(hConsole, GREEN);
			PrintChar(mcsm.column, mcsm.row + i, ptrOut);
		}
	}
};

MatrixCodeView programLines[programLinesMax];
MatrixCodeStaticView staticLines[staticLinesMax];

std::string GetRandomChar()
{
	std::string str = "!";
	str.at(0) = (char)(rand() % 221 + 33);
	if ((char)str.at(0) == (char)127) { str.at(0) = (char)128; }
	return str;
}



void Initialise()
{
	srand(time(NULL)); // sets the seed for RNG
	for (int r : randomArray)
	{
		r = rand();
	}

	std::string str = " ";
	for (unsigned int i = 0; i < screenColums * screenRows; i++)
	{
		clr.append(str);
	}

	for (char i = 0; i < programLinesMax; i++) programLines[i] = MatrixCodeView(MatrixCodeModel());
	for (char i = 0; i < staticLinesMax; i++) staticLines[i] = MatrixCodeStaticView(MatrixCodeStaticModel());
}
 
void Cls()
{
	// cls is a hack, try own cls or manage characters per update
}

void Draw()
{
	Cls();
	for (MatrixCodeStaticView& mcsv : staticLines)
	{
		mcsv.Draw();
		//mcsv.mcsm.Step();
	}
	for (MatrixCodeView& mcv : programLines)
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
		tick++;
	}
	system("cls");
	return 0;
}