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

class MatrixCodeView;
class MatrixCodeModel;
class MatrixCodeStaticView;
class MatrixCodeStaticModel;

const short screenColums = 119;
const short screenRows = 29;
std::string clr = "";
std::string space = " ";
int randomArray[10];

unsigned char tick = 0;
unsigned int programUpdateStep = 75; //ms
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

//const char* lols = " ";

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
		unsigned char strLength = (unsigned char)local[n].length();
		char half = (char)round(strLength / 2);
		
		std::string empty = " ";
		strOut[0] = empty.at(0);
		PrintChar(mcm.column, mcm.row - 1, ptrOut);

		for (char i = strLength - 1; i >= 0; i--)
		{
			if (mcm.row + i < 0 || mcm.row + i > screenRows) continue;
			
			strOut[0] = local[n].at(i);
			if(i == strLength - 1) SetConsoleTextAttribute(hConsole, WHITE);
			else if(i == strLength - 2) SetConsoleTextAttribute(hConsole, CYAN);
			else SetConsoleTextAttribute(hConsole, LIGHTGREEN);
			
			PrintChar(mcm.column, mcm.row + i, ptrOut);
		}
	}
};

class MatrixCodeStaticModel {
public:
	short column = 0;
	short row = 0;
	short entrophy = 0;
	short speed = 0;
	MatrixCodeStaticModel() {};
	MatrixCodeStaticModel(const short& _column) : column(_column) { ReRoll(true); }
	void ReRoll(bool init)
	{
		row = RandomInt() % screenRows;
		speed = (RandomInt() % 4) + 3;
		//column = RandomInt() % screenColums - (lineLengthMax / 2); // the columns cannot be random, use an index from the constructor
		entrophy = lineLengthMax + (RandomInt() % 12);
		// entrophy probablistics short/medium/long
		return;
	}
	void Step()
	{
		if (tick % speed == 0) entrophy--;
		if (entrophy < 0) ReRoll(false);
	}
};

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
		mcsm.ReRoll(true);
	}
	void Draw()
	{
		size_t tLen = trail.length();
		for(unsigned char i = 0; i < tLen; i++)
		{
			if (mcsm.entrophy > i && i < tLen) strOut[0] = trail.at(i);
			else strOut[0] = space.at(0);
			SetConsoleTextAttribute(hConsole, GREEN);
			PrintChar(mcsm.column, mcsm.row - i, ptrOut);
		}
	}
};

MatrixCodeView programLines[programLinesMax];
MatrixCodeStaticView staticLines[staticLinesMax];

std::string GetRandomChar()
{
	std::string str = "!";
	str.at(0) = (unsigned char)(rand() % 221 + 33);
	if ((str.at(0) == (unsigned char)127)
		|| (str.at(0) == (unsigned char)128)
		|| (str.at(0) == (unsigned char)129)
		|| (str.at(0) == (unsigned char)130)
		|| (str.at(0) == (unsigned char)131)
		|| (str.at(0) == (unsigned char)132)
		|| (str.at(0) == (unsigned char)133)
		|| (str.at(0) == (unsigned char)134)
		|| (str.at(0) == (unsigned char)135)
		|| (str.at(0) == (unsigned char)136)
		|| (str.at(0) == (unsigned char)137)
		|| (str.at(0) == (unsigned char)138)
		|| (str.at(0) == (unsigned char)139)
		|| (str.at(0) == (unsigned char)140)
		|| (str.at(0) == (unsigned char)141)
		|| (str.at(0) == (unsigned char)142)
		|| (str.at(0) == (unsigned char)143)
		|| (str.at(0) == (unsigned char)144)
		|| (str.at(0) == (unsigned char)145)
		|| (str.at(0) == (unsigned char)146)
		|| (str.at(0) == (unsigned char)147)
		|| (str.at(0) == (unsigned char)148)
		|| (str.at(0) == (unsigned char)149)
		|| (str.at(0) == (unsigned char)150)
		|| (str.at(0) == (unsigned char)151)
		|| (str.at(0) == (unsigned char)152)
		|| (str.at(0) == (unsigned char)153)
		|| (str.at(0) == (unsigned char)154)
		|| (str.at(0) == (unsigned char)155)
		|| (str.at(0) == (unsigned char)156)
		|| (str.at(0) == (unsigned char)157)
		|| (str.at(0) == (unsigned char)158)
		|| (str.at(0) == (unsigned char)159)
		) { str.at(0) = (unsigned char)55; }
	return str;
}

void Initialise()
{
	srand((unsigned int)time(NULL)); // sets the seed for RNG
	for (int r : randomArray)
	{
		r = rand();
	}

	for (unsigned int i = 0; i < screenColums * screenRows; i++)
	{
		clr.append(space);
	}

	
	

	for (char i = 0; i < programLinesMax; i++) programLines[i] = MatrixCodeView(MatrixCodeModel());
	for (unsigned char i = 0; i < staticLinesMax; i++) { 
		float fraction = (float)(screenColums / staticLinesMax);
		short s = (short)(i * fraction);
		staticLines[i] = MatrixCodeStaticView(MatrixCodeStaticModel(s));
	};
}

void Draw()
{
	// background
	for (MatrixCodeStaticView& mcsv : staticLines)
	{
		mcsv.Draw();
		mcsv.mcsm.Step();
	}
	// foreground
	//for (MatrixCodeView& mcv : programLines)
	//{
	//	mcv.Draw();
	//	mcv.mcm.Step();
	//}



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