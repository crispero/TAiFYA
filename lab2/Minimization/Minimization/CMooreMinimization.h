#pragma once
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <iostream>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>

class CMooreMinimization
{
public:
	CMooreMinimization(const int x, const int y, const int s);
	~CMooreMinimization() = default;

	void Parse(std::ifstream& fin);
	void MinimizationStart();
	void Print(std::vector<int>& finalOutputs, std::vector<std::vector<int>>& outputMatrix, int k);
	void Visualization(int k, std::vector<std::vector<int>>& outputMatrix);
private:
	void Minimization(std::map<int, std::vector<int>>& fmap);
	void PrepareToPrint(std::map<int, std::vector<int>>& fmap);
	std::vector<int> GetFinalOutputs(std::vector<int>& temp);
	int GetNumberFromInput(std::ifstream& fin);
	std::vector<int> GetUnique(const std::vector<int> arr);

	int m_x;
	int m_y;
	int m_s;
	std::vector<int> m_outputAlphabetCharacters;
	std::vector<std::vector<int>> m_inputMatrix;
};

