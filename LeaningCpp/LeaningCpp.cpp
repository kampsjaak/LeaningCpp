#include <iostream>
#include <string.h> // string manipulation
#include <chrono> // delays
#include <thread> // delays
#include <windows.h > // input https://visualstudioclient.gallerycdn.vsassets.io/extensions/visualstudioclient/microsoftvisualstudio2017installerprojects/1.0.0/1620063166533/InstallerProjects.vsix
#include <ctime> // random nummer generation

class MatrixCode {
	private:
		short column = 1;
		short row = 0;
		const char* sLiteral = "immutable";
		char trail[5];
		unsigned int speed = 0; // 1, 2, 3
		const char* GenerateString()
		{
			char newStr[] = "mutable";
			//newStr[2] = 'c';
			for (int i = 0; i < 5; i++)
			{
				newStr[i]= '1';
			}

			return newStr;
		}
	public:
		void Initilise(short _column, int _speed)
		{
			column = _column;
			speed = _speed;
		}
		void Step()
		{
			row = row + speed;
		}
};

short colums = 20;
short rows = 50;
bool bUpdate = true;
unsigned int microseconds = 300;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD pos = { 32, 12 };

int main()
{
	srand(time(NULL)); // sets the seed for RNG
	pos = { (short)(abs(rand()) % 50), (short)(abs(rand()) % 30) };
	SetConsoleCursorPosition(hConsole, pos);

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		WriteConsole(hConsole, "!", 1, NULL, NULL);
		std::this_thread::sleep_for(std::chrono::milliseconds(microseconds));
	}
	return 0;
}

void printChar(short _x, short _y, char _char)
{

}