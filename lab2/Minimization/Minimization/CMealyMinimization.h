#pragma once
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <iostream>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>

struct SAndY
{
	int s;
	int y;
};

class CMealyMinimization
{
public:
	CMealyMinimization(const int x, const int y, const int s);
	~CMealyMinimization() = default;

	void Parse(std::ifstream& fin);
	void Print(std::vector<std::vector<std::pair<int, int>>>& outputMatrix, int k);
	void Visualization(int k, std::vector<std::vector<std::pair<int, int>>>& outputMatrix);
private:
	SAndY GetTwoNumbersFromInput(std::ifstream& fin);
	std::vector<std::vector<int>> GetUnique(std::vector<std::vector<int>> arr);
	void Minimization(std::map<int, std::vector<int>>& fmap);
	void PrepareToPrint(std::map<int, std::vector<int>>& fmap);

	int m_x;
	int m_y;
	int m_s;
	std::vector<std::vector<std::pair<int, int>>> m_inputMatrix;
};

