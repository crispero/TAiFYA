#pragma once
#include <regex>
#include <fstream>
#include <vector>
#include <map>

struct SAndY
{
	int s;
	int y;
};

int GetNumberFromInput(std::ifstream& fin)
{
	std::string line;
	fin >> line;
	std::smatch res;
	std::regex reg("[0-9]\\d*");
	if (std::regex_search(line, res, reg))
	{
		return std::stoi(res[0]);
	}
}

SAndY GetTwoNumbersFromInput(std::ifstream& fin)
{
	SAndY dto;
	std::string line;
	fin >> line;
	std::smatch res;
	std::regex reg("[0-9]\\d*");
	if (std::regex_search(line, res, reg))
	{
		dto.s = std::stoi(res[0]);
		line = res.suffix();
		if (std::regex_search(line, res, reg))
		{
			dto.y = std::stoi(res[0]);
			return dto;
		}
	}
}

std::vector<int> GetUnique(const std::vector<int> arr)
{
	std::vector<int> unique;
	unique.resize(arr.size());
	std::copy(arr.begin(), arr.end(), unique.begin());
	std::sort(unique.begin(), unique.end());
	unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
	return unique;
}
