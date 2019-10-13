#include "CMealyMinimization.h"
#include "CMooreMinimization.h"
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Arguments error" << std::endl;
		return 1;
	}

	std::string fileName = argv[1];
	std::ifstream fin(fileName);

	if (!fin.is_open())
	{
		std::cout << "Can`t open file" << std::endl;
		return 1;
	}

	int x, y, s;
	std::string type;
	fin >> x >> y >> s >> type;

	if (type == "mealy")
	{
	//	CMealyMinimization mealyMinimization(x, y, s);
	}
	else if (type == "moore")
	{
		CMooreMinimization mooreMinimization(x, y, s);
		mooreMinimization.Parse(fin);
		mooreMinimization.MinimizationStart();
	}
	else
	{
		std::cout << "Unknown type" << std::endl;
		return 1;
	}

	return 0;
}

