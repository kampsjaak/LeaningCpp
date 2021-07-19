#include <iostream>
#include <string.h>

unsigned int colums = 20;
unsigned int rows = 50;
bool bUpdate = true;
unsigned int myClock = 0;

void printChar(int _c, int _r, char _char)
{

}

char RandomChar()
{
	return rand() % 255;
}

class MatrixCode {
	public:
		int publicParm = 1337;
	private:
		unsigned int column = 1;
		unsigned int speed = 0; // 1, 2, 3
		const char* sLiteral = "immutable";
		std::string strTrail = "abcdef";
		char trail[5];
		
		void GenerateString()
		{
			for (int i = 0; i < 3; i = i + 1)
			{
				strTrail.replace(i, 1, (const char*)RandomChar());
			}
			std::cout << strTrail << std::endl;
		}
	public:
		void Initalise(int _column, int _speed)
		{
			column = _column;
			speed = _speed;
			GenerateString();
		}
};



int main()
{
	MatrixCode obj;
	obj.Initalise(1, 2);

	while (bUpdate)
	{
		if (myClock % 10 == 0)
		{

		}
		myClock = myClock + 1;
	}
	return 0;
}

