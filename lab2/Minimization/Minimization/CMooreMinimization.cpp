#include "CMooreMinimization.h"
#include "Helper.h"
#include <iostream>

CMooreMinimization::CMooreMinimization(const int x, const int y, const int s)
	: m_x(x)
	, m_y(y)
	, m_s(s)
{
}


void CMooreMinimization::Parse(std::ifstream& fin)
{
	do
	{
		m_outputAlphabetCharacters.push_back(GetNumberFromInput(fin));
	} while (fin.peek() != '\n');

	m_inputMatrix.resize(m_x);
	for (size_t i = 0; i < m_x; i++)
	{
		m_inputMatrix[i].resize(m_s);
		for (size_t j = 0; j < m_s; j++)
		{
			m_inputMatrix[i][j] = GetNumberFromInput(fin);
		}
	}
}

void CMooreMinimization::MinimizationStart()
{
	auto uniqueOutput = GetUnique(m_outputAlphabetCharacters);
	std::map<int, std::vector<int>> fmap;
	for (size_t i = 0; i < uniqueOutput.size(); ++i)
	{
		std::vector<int> temp;
		for (size_t j = 0; j < m_outputAlphabetCharacters.size(); ++j)
		{
			if (m_outputAlphabetCharacters[j] == uniqueOutput[i])
			{
				temp.push_back(j);
			}
		}
		fmap.insert(std::make_pair(uniqueOutput[i], temp));
	}
	Minimization(fmap);
}

void CMooreMinimization::Minimization(std::map<int, std::vector<int>>& fmap)
{
	bool stopMinimization = false;
	std::vector<std::vector<int>> newMatrix(m_x);
	std::vector<size_t> mapSizes;
	
	std::map<int, std::vector<std::pair<int, std::vector<int>>>> map1;
	for (auto it = fmap.begin(); it != fmap.end(); ++it)
	{
		mapSizes.push_back(it->second.size());
		std::vector<std::pair<int, std::vector<int>>> vv;
		for (size_t i = 0; i < it->second.size(); ++i)
		{
			std::vector<int> temp;
			for (size_t j = 0; j < m_x; ++j)
			{
				
				newMatrix[j].resize(m_s);
				auto a = m_inputMatrix[j][it->second[i]];
				for (auto it1 = fmap.begin(); it != fmap.end(); ++it1)
				{
					if (std::find(it1->second.begin(), it1->second.end(), a) != it1->second.end())
					{
						temp.push_back(it1->first);
						newMatrix[j][i] = it1->first;
						break;
					}
				}
			}
			vv.push_back(std::make_pair(it->second[i], temp));
		}
		map1.insert(std::make_pair(it->first, vv));
	}

	std::map<int, std::vector<int>> outmap;
	int index = 0;
	for (auto it = map1.begin(); it != map1.end(); ++it)
	{
		for (size_t i = 0; i < it->second.size(); ++i)
		{
			std::vector<int> temp;
			for (size_t j = 0; j < it->second.size(); ++j)
			{
				if (it->second[i].second == it->second[j].second)
				{
					temp.push_back(it->second[j].first);
				}
			}
			if (outmap.size() != 0)
			{
				for (auto it1 = outmap.begin(); it1 != outmap.end(); ++it1)
				{
					if (it1->second == temp)
					{
						outmap.insert(std::make_pair(index, temp));
						index++;
					}
				}
			}
			else
			{
				outmap.insert(std::make_pair(index, temp));
				index++;
			}
		}
	}
	
	std::cout << std::endl;
}