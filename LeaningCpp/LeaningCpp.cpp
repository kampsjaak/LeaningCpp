#include <iostream>
#include <string.h>

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
long unsigned int clock = 0;


int main()
{
	
	while (bUpdate)
	{
		if (clock % 10 == 0)
		{

		}
		clock++;
	}
	return 0;
}

void printChar(int _c, int _r, char _char)
{

}