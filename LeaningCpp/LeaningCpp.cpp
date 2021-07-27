#include <iostream>
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
CONSOLE_CURSOR_INFO cursorInfo;

class MatrixCodeView;
class MatrixCodeModel;
class MatrixCodeStaticView;
class MatrixCodeStaticModel;

const unsigned char programLinesMax = 20;
const unsigned char staticLinesMax = 35;
const unsigned char lineLengthMin = 5;
const unsigned char lineLengthMax = 21;
const unsigned char illegal[] = { 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
		139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
		156, 157, 158, 159 };
const unsigned short screenColums = 119;
const unsigned short screenRows = 29;
const unsigned int programUpdateStep = 75; //ms

std::string clr = "";
std::string space = " ";
char strOut[1] = { 'S' };
char* ptrOut = strOut;
unsigned char tick = 0;
unsigned char cycle = 0;
int randomArray[10];
int temp;

std::string GetRandomChar();
short ColumnContainsStaticLine(short _col);

int RandomInt()
{
	return rand();
	
	//random enough
	//add randoms together
	//substract randoms
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

class MatrixCodeStaticModel {
public:
	short column = 0;
	short row = 0;
	short entrophy = 0;
	short speed = 0;
	MatrixCodeStaticModel() {};
	MatrixCodeStaticModel(const short& _column) : column(_column) { }
	void ReRoll(bool roll)
	{
		row = RandomInt() % screenRows;
		speed = (RandomInt() % 4) + 3;
		if (roll) entrophy = RandomInt() % 30;
		else entrophy = lineLengthMax + (RandomInt() % 12) + 12;
		// entrophy probablistics short/medium/long
		return;
	}
	void Step()
	{
		if (tick % speed == 0) entrophy--;
		if (entrophy < -10) ReRoll(false);
	}
};

class MatrixCodeStaticView {
private:
	bool highLight = false;
	unsigned char highLightIndex = 0;
	char highLightCountDown = 0;
	std::string trail = "abcdefghijklm";
public:
	bool updateDraw = true;
	MatrixCodeStaticModel mcsm;
	MatrixCodeStaticView() {};
	MatrixCodeStaticView(const MatrixCodeStaticModel& _mcsm) : mcsm(_mcsm) { Initialise();  };
	void Initialise()
	{
		trail = GenerateString(trail);
		mcsm.ReRoll(true);
	}
	void Draw()
	{
		if (!updateDraw) return;

		for(unsigned char i = 0; i < trail.length(); i++)
		{
			if (mcsm.entrophy > i && i < trail.length()) strOut[0] = trail.at(i);
			else strOut[0] = space.at(0);
			
			if (highLight && highLightCountDown == tick) highLight = false;
			if (5 == i + (tick % 20) + mcsm.entrophy)
			{
				highLight = true; 
				highLightIndex = i; 
				highLightCountDown = tick + 10;
			}			
			if (highLight && i == highLightIndex) SetConsoleTextAttribute(hConsole, WHITE);
			else SetConsoleTextAttribute(hConsole, GREEN);
			
			PrintChar(mcsm.column, mcsm.row - i, ptrOut);
		}
	}
};

MatrixCodeStaticView staticLines[staticLinesMax];

class MatrixCodeModel {
private:
	short disabledStaticLineIndex = -1;
public:
	short column;
	short row = RandomInt() % screenRows;
	MatrixCodeModel() { ReRoll(true); }
	void ReRoll(bool init)
	{
		if (disabledStaticLineIndex > 0) staticLines[disabledStaticLineIndex].updateDraw = true;

		int random = RandomInt();
		column = random % screenColums;
		if (!init) row = -(lineLengthMax + (random % lineLengthMax));

		disabledStaticLineIndex = ColumnContainsStaticLine(column);
		if (disabledStaticLineIndex > 0) staticLines[disabledStaticLineIndex].updateDraw = false;
	}
	void Step()
	{
		if (row > screenRows) ReRoll(false);
		else row++;
	}
};

class MatrixCodeView {
private:
	std::string trailA = "abcdefghijklmnopqrstu";
	std::string trailB = "abcdefghijklmnopqrstu";
	std::string trailC = "abcdefghijklmnopqrstu";
	std::string trailD = "abcdefghijklmnopqrstu";
public:
	MatrixCodeModel mcm;
	MatrixCodeView() {};
	MatrixCodeView(const MatrixCodeModel& _mcm) : mcm(_mcm) { Initialise(); };

	void Initialise()
	{	// pre-generate strings so we used less rand()
		trailA = GenerateString(trailA);
		trailB = GenerateString(trailB);
		trailC = GenerateString(trailC);
		trailD = GenerateString(trailD);
	}
	void Draw()
	{
		char n = tick % 4;
		std::string local[4] = { trailA, trailB, trailC, trailD };
		unsigned char strLength = (unsigned char)local[n].length();
		char half = (char)round(strLength / 2);

		std::string empty = " ";
		strOut[0] = empty.at(0);
		PrintChar(mcm.column, mcm.row - 1, ptrOut);

		for (char i = strLength - 1; i >= 0; i--)
		{
			if (mcm.row + i < 0 || mcm.row + i > screenRows) continue;

			strOut[0] = local[n].at(i);
			if (i == strLength - 1) SetConsoleTextAttribute(hConsole, WHITE);
			else if (i == strLength - 2) SetConsoleTextAttribute(hConsole, CYAN);
			else SetConsoleTextAttribute(hConsole, LIGHTGREEN);

			PrintChar(mcm.column, mcm.row + i, ptrOut);
		}
	}
};

MatrixCodeView programLines[programLinesMax];

bool IsLegalUnsignedChar(unsigned char _c)
{
	for (unsigned char i : illegal)
		if (i == _c) return false;
	
	return true;
}

short ColumnContainsStaticLine(short _col)
{
	for (unsigned char i = 0; i < staticLinesMax; i++)
	{
		if (staticLines[i].mcsm.column == _col) return i;
	}
	return -1;
}

std::string GetRandomChar()
{
	std::string str = "!";
	str.at(0) = (unsigned char)(rand() % 221 + 33);
	if (!IsLegalUnsignedChar(str.at(0))) str.at(0) = (unsigned char)55;
	return str;
}

void Initialise()
{
	srand((unsigned int)time(NULL)); // sets the seed for RNG
	
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	for (int r : randomArray)
	{
		r = rand();
	}

	for (unsigned int i = 0; i < screenColums * screenRows; i++)
	{
		clr.append(space);
	}

	for (unsigned char i = 0; i < staticLinesMax; i++) { 
		float fraction = (float)(screenColums / staticLinesMax);
		short s = (short)(i * fraction);
		staticLines[i] = MatrixCodeStaticView(MatrixCodeStaticModel(s));
	};
	for (char i = 0; i < programLinesMax; i++) programLines[i] = MatrixCodeView(MatrixCodeModel());
}

void Draw()
{
	for (MatrixCodeStaticView& mcsv : staticLines) // background
	{
		mcsv.Draw();
		mcsv.mcsm.Step();
	}
	for (MatrixCodeView& mcv : programLines) // foreground
	{
		mcv.Draw();
		mcv.mcm.Step();
	}

	SetConsoleCursorPosition(hConsole, cursorPos);
}

int main()
{
	system("cls");
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