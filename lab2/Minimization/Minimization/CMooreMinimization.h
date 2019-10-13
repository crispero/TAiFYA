#pragma once

class CMooreMinimization
{
public:
	CMooreMinimization(const int x, const int y, const int s);
	~CMooreMinimization() = default;

	void Parse(std::ifstream& fin);
	void MinimizationStart();
	void Print(std::vector<int>& finalOutputs, std::vector<std::vector<int>>& outputMatrix, int k);
private:
	void Minimization(std::map<int, std::vector<int>>& fmap);
	void PrepareToPrint(std::map<int, std::vector<int>>& fmap);
	std::vector<int> GetFinalOutputs(std::vector<int>& temp);

	int m_x;
	int m_y;
	int m_s;
	std::vector<int> m_outputAlphabetCharacters;
	std::vector<std::vector<int>> m_inputMatrix;
};

