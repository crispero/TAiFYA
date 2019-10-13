#include <regex>

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

std::vector<int> GetUnique(const std::vector<int> arr)
{
	std::vector<int> unique;
	unique.resize(arr.size());
	std::copy(arr.begin(), arr.end(), unique.begin());
	std::sort(unique.begin(), unique.end());
	unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
	return unique;
}
