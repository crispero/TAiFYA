#include "CLexer.h"
#include <iostream>

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			throw std::exception("Use input.txt and output.txt");
		}

		std::ifstream fin(argv[1]);
		std::ofstream fout(argv[2]);

		if (!fin.is_open())
		{
			throw std::exception("Input file is closed");
		} 

		CLexer lexer(fin);
		lexer.StartProcess();
		lexer.PrintTokens(std::cout);

	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}