#include "CMealyMinimization.h"
#include "CMooreMinimization.h"
#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			throw std::exception("Invalid arguments count");
		}

		std::string fileName = argv[1];
		std::ifstream fin(fileName);
		std::ofstream fout(argv[2]);

		if (!fin.is_open())
		{
			throw std::exception("Can't open file");
		}

		int x, y, s;
		std::string type;
		fin >> x >> y >> s >> type;

		if (type == "mealy")
		{
			CMealyMinimization mealyMinimization(x, y, s);
			mealyMinimization.Parse(fin);
		}
		else if (type == "moore")
		{
			CMooreMinimization mooreMinimization(x, y, s);
			mooreMinimization.Parse(fin);
			mooreMinimization.Minimization();
			mooreMinimization.Print(fout);
			mooreMinimization.Visualization();
		}
		else
		{
			throw std::exception("Unknown type");
		}
	} 
	catch (std::exception & e)
	{
		std::cout << e.what() << '\n';
	}
	

	return 0;
}

