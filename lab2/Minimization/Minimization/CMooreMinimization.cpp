#include "CMooreMinimization.h"

CMooreMinimization::CMooreMinimization(const size_t x, const size_t y, const size_t s)
	: m_x(x)
	, m_y(y)
	, m_s(s)
{
}

void CMooreMinimization::Parse(std::istream& fin)
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

	SetMapForMinimization();
}

int CMooreMinimization::GetNumberFromInput(std::istream& fin)
{
	std::string line;
	fin >> line;
	std::smatch res;
	std::regex reg("[0-9]\\d*");
	if (std::regex_search(line, res, reg))
	{
		return std::stoi(res[0]);
	}
	else
	{
		throw std::exception("Failed to parse number from input");
	}
}

std::vector<int> CMooreMinimization::GetUnique(const std::vector<int>& arr)
{
	std::vector<int> unique;
	unique.resize(arr.size());
	std::copy(arr.begin(), arr.end(), unique.begin());
	std::sort(unique.begin(), unique.end());
	unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
	return unique;
}

void CMooreMinimization::SetMapForMinimization()
{
	auto uniqueOutput = GetUnique(m_outputAlphabetCharacters);
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
		m_mapForMinimization.insert(std::make_pair(uniqueOutput[i], temp));
	}
}

std::map<int, std::vector<std::pair<int, std::vector<int>>>> CMooreMinimization::GetDontKnowWhatKindOfMap()
{
	std::map<int, std::vector<std::pair<int, std::vector<int>>>> dontKnowWhatKindOfMap;
	for (auto it = m_mapForMinimization.begin(); it != m_mapForMinimization.end(); ++it)
	{
		std::vector<std::pair<int, std::vector<int>>> vv;
		for (size_t i = 0; i < it->second.size(); ++i)
		{
			std::vector<int> temp;
			for (size_t j = 0; j < m_x; ++j)
			{
				auto a = m_inputMatrix[j][it->second[i]];
				for (auto it1 = m_mapForMinimization.begin(); it != m_mapForMinimization.end(); ++it1)
				{
					if (std::find(it1->second.begin(), it1->second.end(), a) != it1->second.end())
					{
						temp.push_back(it1->first);
						break;
					}
				}
			}
			vv.push_back(std::make_pair(it->second[i], temp));
		}
		dontKnowWhatKindOfMap.insert(std::make_pair(it->first, vv));
	}
	return dontKnowWhatKindOfMap;
}

std::map<int, std::vector<int>> CMooreMinimization::GetTempMapForMinimization(const std::map<int, std::vector<std::pair<int, std::vector<int>>>>& dontKnowWhatKindOfMap)
{
	std::map<int, std::vector<int>> tempMapForMinimization;
	int index = 0;
	for (auto it = dontKnowWhatKindOfMap.begin(); it != dontKnowWhatKindOfMap.end(); ++it)
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
			tempMapForMinimization.insert(std::make_pair(index, temp));
			index++;
		}
	}
	return tempMapForMinimization;
}

void CMooreMinimization::Minimization()
{	
	std::map<int, std::vector<std::pair<int, std::vector<int>>>> dontKnowWhatKindOfMap = GetDontKnowWhatKindOfMap();
	
	std::map<int, std::vector<int>> tempMapForMinimization = GetTempMapForMinimization(dontKnowWhatKindOfMap);
	
	
	std::vector<std::vector<int>> temp;
	for (auto it = tempMapForMinimization.begin(); it != tempMapForMinimization.end(); ++it)
	{
		temp.push_back(it->second);
	}

	tempMapForMinimization.clear();
	tempMapForMinimization.insert(m_mapForMinimization.begin(), m_mapForMinimization.end());
	m_mapForMinimization.clear();

	std::vector<std::vector<int>> unique;
	unique.resize(temp.size());
	std::copy(temp.begin(), temp.end(), unique.begin());
	std::sort(unique.begin(), unique.end());
	unique.erase(std::unique(unique.begin(), unique.end()), unique.end());

	for (size_t i = 0; i < unique.size(); ++i)
	{
		m_mapForMinimization.insert(std::make_pair(i, unique[i]));
	}

	if (m_mapForMinimization.size() != tempMapForMinimization.size())
	{
		Minimization();
	} 
	else
	{
		PrepareToPrint();
	}
}

void CMooreMinimization::PrepareToPrint()
{
	m_outputMatrix.resize(m_x);
	std::vector<int> temp;
	for (auto it = m_mapForMinimization.begin(); it != m_mapForMinimization.end(); ++it)
	{
		temp.push_back(it->second[0]);
	}

	m_finalOutputs = GetFinalOutputs(temp);

	for (size_t i = 0; i < m_x; ++i)
	{
		m_outputMatrix[i].resize(m_mapForMinimization.size());
		for (auto it = m_mapForMinimization.begin(); it != m_mapForMinimization.end(); ++it)
		{
			auto s = m_inputMatrix[i][it->second[0]];
			for (auto it1 = m_mapForMinimization.begin(); it1 != m_mapForMinimization.end(); ++it1)
			{
				auto index = std::find(it1->second.begin(), it1->second.end(), s);
				if (index != it1->second.end())
				{
					m_outputMatrix[i][it->first] = it1->first;
					break;
				}
			}
		}
	}

	SetVisualizationVector();
}

std::vector<int> CMooreMinimization::GetFinalOutputs(const std::vector<int>& temp)
{
	std::vector<int> finalOutputs;
	for (size_t i = 0; i < temp.size(); ++i)
	{
		finalOutputs.push_back(m_outputAlphabetCharacters[temp[i]]);
	}
	return finalOutputs;
}

void CMooreMinimization::Print(std::ostream& fout) const
{
	for (size_t i = 0; i < m_finalOutputs.size(); ++i)
	{
		fout << "y" << m_finalOutputs[i] << " ";
	}

	fout << std::endl;

	for (size_t i = 0; i < m_x; ++i)
	{
		for (size_t j = 0; j < m_mapForMinimization.size(); ++j)
		{
			fout << "z" << m_outputMatrix[i][j] << " ";
		}
		fout << std::endl;
	}
}

void CMooreMinimization::SetVisualizationVector()
{
	for (size_t i = 0; i < m_outputMatrix.size(); ++i)
	{
		for (size_t j = 0; j < m_outputMatrix[i].size(); ++j)
		{
			m_visualizationVector.push_back(m_outputMatrix[i][j]);
		}
	}
}

void CMooreMinimization::Visualization() const
{
	using Edge = std::pair<int, int>;
	using Graph = boost::adjacency_list< boost::vecS,
		boost::vecS, boost::directedS,
		boost::property<boost::vertex_color_t,
		boost::default_color_type>,
		boost::property< boost::edge_weight_t, std::string>
	>;
	
	std::vector<std::string> weights(m_visualizationVector.size());
	std::vector<std::pair<int, int>> edge(m_visualizationVector.size());
	std::ofstream fout("mooreGraph.dot");

	for (size_t i = 0, x = 0, index = 0; i < m_visualizationVector.size(); ++i, ++index)
	{
		if (i % m_finalOutputs.size() == 0 && i != 0)
		{
			++x;
			index = 0;
		}
		weights[i] = 'x' + std::to_string(x);
		edge[i] = { index, m_visualizationVector[i] };
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), m_finalOutputs.size());

	boost::dynamic_properties dp;
	dp.property("label", boost::get(boost::edge_weight, graph));
	dp.property("node_id", boost::get(boost::vertex_index, graph));

	write_graphviz_dp(fout, graph, dp);
}