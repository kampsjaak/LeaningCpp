#include <iostream>
#include <string.h> // string manipulation
#include <chrono> // delays
#include <thread> // delays
#include <windows.h > // input https://visualstudioclient.gallerycdn.vsassets.io/extensions/visualstudioclient/microsoftvisualstudio2017installerprojects/1.0.0/1620063166533/InstallerProjects.vsix

class MatrixCode {
	private:
		unsigned int column = 1;
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
		void Initilise(int _column, int _speed)
		{
			column = _column;
			speed = _speed;
		}
};

unsigned int colums = 20;
unsigned int rows = 50;
bool bUpdate = true;
long unsigned int myClock = 0;
unsigned int microseconds = 300;

int main()
{
	while (!GetAsyncKeyState(VK_ESCAPE))
	{		
		if (myClock == 20) return 0;
		std::cout << '!';
		std::this_thread::sleep_for(std::chrono::milliseconds(microseconds));
	}
	return 0;
}

void printChar(int _c, int _r, char _char)
{

}