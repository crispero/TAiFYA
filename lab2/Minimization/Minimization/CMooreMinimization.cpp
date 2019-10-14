#include "CMooreMinimization.h"

CMooreMinimization::CMooreMinimization(const int x, const int y, const int s)
	: m_x(x)
	, m_y(y)
	, m_s(s)
{
}

int CMooreMinimization::GetNumberFromInput(std::ifstream& fin)
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

std::vector<int> CMooreMinimization::GetUnique(const std::vector<int> arr)
{
	std::vector<int> unique;
	unique.resize(arr.size());
	std::copy(arr.begin(), arr.end(), unique.begin());
	std::sort(unique.begin(), unique.end());
	unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
	return unique;
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
			outmap.insert(std::make_pair(index, temp));
			index++;
		}
	}
	
	std::vector<std::vector<int>> temp;
	for (auto it = outmap.begin(); it != outmap.end(); ++it)
	{
		temp.push_back(it->second);
	}

	std::vector<std::vector<int>> unique;
	unique.resize(temp.size());
	std::copy(temp.begin(), temp.end(), unique.begin());
	std::sort(unique.begin(), unique.end());
	unique.erase(std::unique(unique.begin(), unique.end()), unique.end());

	outmap.clear();
	for (size_t i = 0; i < unique.size(); ++i)
	{
		outmap.insert(std::make_pair(i, unique[i]));
	}

	if (fmap.size() != outmap.size())
	{
		Minimization(outmap);
	} 
	else
	{
		PrepareToPrint(outmap);
	}
}

void CMooreMinimization::PrepareToPrint(std::map<int, std::vector<int>>& outmap)
{
	std::vector<std::vector<int>> outputMatrix(m_x);
	std::vector<int> temp;
	for (auto it = outmap.begin(); it != outmap.end(); ++it)
	{
		temp.push_back(it->second[0]);
	}

	std::vector<int> finalOutputs = GetFinalOutputs(temp);

	for (size_t i = 0; i < m_x; ++i)
	{
		outputMatrix[i].resize(outmap.size());
		for (auto it = outmap.begin(); it != outmap.end(); ++it)
		{
			auto s = m_inputMatrix[i][it->second[0]];
			for (auto it1 = outmap.begin(); it1 != outmap.end(); ++it1)
			{
				auto index = std::find(it1->second.begin(), it1->second.end(), s);
				if (index != it1->second.end())
				{
					outputMatrix[i][it->first] = it1->first;
					break;
				}
			}
		}
	}

	Print(finalOutputs, outputMatrix, outmap.size());
	Visualization(finalOutputs.size(), outputMatrix);
}

std::vector<int> CMooreMinimization::GetFinalOutputs(std::vector<int>& temp)
{
	std::vector<int> finalOutputs;
	for (size_t i = 0; i < temp.size(); ++i)
	{
		finalOutputs.push_back(m_outputAlphabetCharacters[temp[i]]);
	}
	return finalOutputs;
}

void CMooreMinimization::Print(std::vector<int>& finalOutputs, std::vector<std::vector<int>>& outputMatrix, int k)
{
	for (size_t i = 0; i < finalOutputs.size(); ++i)
	{
		std::cout << "y" << finalOutputs[i] << " ";
	}

	std::cout << std::endl;

	for (size_t i = 0; i < m_x; ++i)
	{
		for (int j = 0; j < k; ++j)
		{
			std::cout << "z" << outputMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void CMooreMinimization::Visualization(int k, std::vector<std::vector<int>>& outputMatrix)
{
	using Edge = std::pair<int, int>;
	using Graph = boost::adjacency_list< boost::vecS,
		boost::vecS, boost::directedS,
		boost::property<boost::vertex_color_t,
		boost::default_color_type>,
		boost::property< boost::edge_weight_t, std::string>
	>;

	std::vector<int> temp;
	for (size_t i = 0; i < outputMatrix.size(); ++i)
	{
		for (size_t j = 0; j < outputMatrix[i].size(); ++j)
		{
			temp.push_back(outputMatrix[i][j]);
		}
	}

	std::vector<std::string> weights(temp.size());
	std::vector<std::pair<int, int>> edge(temp.size());
	std::ofstream fout("mooreGraph.dot");

	for (size_t i = 0, x = 0, index = 0; i < temp.size(); ++i, ++index)
	{
		if (i % k == 0 && i != 0)
		{
			++x;
			index = 0;
		}
		weights[i] = 'x' + std::to_string(x);
		edge[i] = { index, temp[i] };
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), k);

	boost::dynamic_properties dp;
	dp.property("label", boost::get(boost::edge_weight, graph));
	dp.property("node_id", boost::get(boost::vertex_index, graph));

	write_graphviz_dp(fout, graph, dp);
}