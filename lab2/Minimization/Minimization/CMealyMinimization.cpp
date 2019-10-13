#include "CMealyMinimization.h"

CMealyMinimization::CMealyMinimization(const int x, const int y, const int s)
	: m_x(x)
	, m_y(y)
	, m_s(s)
{
}

void CMealyMinimization::Parse(std::ifstream& fin)
{
	std::vector<std::pair<int, int>> temp;
	while (!fin.eof()) {
		SAndY dto = GetTwoNumbersFromInput(fin);
		temp.push_back(std::make_pair(dto.s, dto.y));
	}

	m_inputMatrix.resize(m_x);
	for (size_t i = 0; i < m_x; ++i)
	{

	}
}