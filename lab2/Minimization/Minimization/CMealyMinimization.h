#pragma once

class CMealyMinimization
{
public:
	CMealyMinimization(const int x, const int y, const int s);
	~CMealyMinimization() = default;

	void Parse(std::ifstream& fin);
private:
	int m_x;
	int m_y;
	int m_s;
	std::vector<std::vector<std::pair<int, int>>> m_inputMatrix;
};

