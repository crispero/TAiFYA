#pragma once
#include <fstream>
#include <vector>
#include <map>

class CMooreMinimization
{
public:
	CMooreMinimization(int x, int y, int s);
	~CMooreMinimization() = default;

	void Parse(std::ifstream& fin);
	void MinimizationStart();
private:
	void Minimization(std::map<int, std::vector<int>>& fmap);

	int m_x;
	int m_y;
	int m_s;
	std::vector<int> m_outputAlphabetCharacters;
	std::vector<std::vector<int>> m_inputMatrix;
};

