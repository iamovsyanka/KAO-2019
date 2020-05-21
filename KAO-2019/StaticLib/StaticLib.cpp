#include <iostream>
#include <cstdlib>
#include <ctime>

extern "C"
{
	int Strlen(char* str)
	{
		if (str == nullptr)  return 0;
		int len = 0;
		for (int i = 0; i < 256; i++)
			if (str[i] == '\0')
			{
				len = i;
				break;
			}

		return len;
	}

	int Random(int number1)
	{
		srand(time(0));
		return 1 + rand() % number1;
	}

	void Line()
	{
		std::cout << std::endl;
	}

	void OutInt(int number)
	{
		std::cout << number;
	}

	void OutStr(char* ptr)
	{
		if (ptr == nullptr)
		{
			std::cout << std::endl;
			return;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
	}
}