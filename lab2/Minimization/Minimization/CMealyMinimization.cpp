#include "CMealyMinimization.h"

CMealyMinimization::CMealyMinimization(const int x, const int y, const int s)
	: m_x(x)
	, m_y(y)
	, m_s(s)
{
}

SAndY CMealyMinimization::GetTwoNumbersFromInput(std::ifstream& fin)
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

void CMealyMinimization::Parse(std::ifstream& fin)
{
	std::vector<std::pair<int, int>> inputData;
	while (!fin.eof()) {
		SAndY dto = GetTwoNumbersFromInput(fin);
		inputData.push_back(std::make_pair(dto.s, dto.y));
	}

	m_inputMatrix.resize(m_x);
	int index = 0;
	for (size_t i = 0; i < m_x; ++i)
	{
		m_inputMatrix[i].resize(inputData.size() / m_x);
		for (size_t j = 0; j < inputData.size() / m_x; ++j)
		{
			m_inputMatrix[i][j] = inputData[index];
			index++;
		}
	}

	std::vector<std::vector<int>> outputOnly;
	outputOnly.resize(inputData.size() / m_x);
	for (size_t i = 0; i < inputData.size() / m_x; ++i)
	{
		outputOnly[i].resize(m_x);
		std::vector<int> temp;
		int a = i;
		for (size_t j = 0; j < m_x; ++j)
		{
			temp.push_back(inputData[a].second);
			a += inputData.size() / m_x;
		}
		outputOnly[i] = temp;
	}

	std::vector<std::vector<int>> unique = GetUnique(outputOnly);

	std::map<int, std::vector<int>> fmap;
	for (size_t i = 0; i < unique.size(); ++i)
	{
		std::vector<int> temp;
		for (size_t j = 0; j < outputOnly.size(); ++j)
		{
			if (unique[i] == outputOnly[j])
			{
				temp.push_back(j);
			}
		}
		fmap.insert(std::make_pair(i, temp));
	}

	Minimization(fmap);
}

std::vector<std::vector<int>> CMealyMinimization::GetUnique(std::vector<std::vector<int>> arr)
{
	std::vector<std::vector<int>> unique;
	unique.resize(arr.size());
	std::copy(arr.begin(), arr.end(), unique.begin());
	std::sort(unique.begin(), unique.end());
	unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
	return unique;
}

void CMealyMinimization::Minimization(std::map<int, std::vector<int>>& fmap)
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
				auto a = m_inputMatrix[j][it->second[i]].first;
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

void CMealyMinimization::PrepareToPrint(std::map<int, std::vector<int>>& outmap)
{
	std::vector<std::vector<std::pair<int, int>>> outputMatrix(m_x);
	std::vector<int> temp;
	for (auto it = outmap.begin(); it != outmap.end(); ++it)
	{
		temp.push_back(it->second[0]);
	}

	for (size_t i = 0; i < m_x; ++i)
	{
		outputMatrix[i].resize(outmap.size());
		for (auto it = outmap.begin(); it != outmap.end(); ++it)
		{
			auto s = m_inputMatrix[i][it->second[0]].first;
			auto y = m_inputMatrix[i][it->second[0]].second;
			for (auto it1 = outmap.begin(); it1 != outmap.end(); ++it1)
			{
				auto index = std::find(it1->second.begin(), it1->second.end(), s);
				if (index != it1->second.end())
				{
					outputMatrix[i][it->first] = std::make_pair(it1->first, y);
					break;
				}
			}
		}
	} 

	Print(outputMatrix, outmap.size());
	Visualization(outmap.size(), outputMatrix);
}

void CMealyMinimization::Print(std::vector<std::vector<std::pair<int, int>>>& outputMatrix, int k)
{
	for (size_t i = 0; i < m_x; ++i)
	{
		for (int j = 0; j < k; ++j)
		{
			std::cout << "s" << outputMatrix[i][j].first << "y" << outputMatrix[i][j].second << " ";
		}
		std::cout << std::endl;
	}
}

void CMealyMinimization::Visualization(int k, std::vector<std::vector<std::pair<int, int>>>& outputMatrix)
{
	using Edge = std::pair<int, int>;
	using Graph = boost::adjacency_list< boost::vecS,
		boost::vecS, boost::directedS,
		boost::property<boost::vertex_color_t,
		boost::default_color_type>,
		boost::property< boost::edge_weight_t, std::string>
	>;
	std::ofstream fout("mealyGraph.dot");

	std::vector<std::pair<int, int>> temp;
	for (size_t i = 0; i < outputMatrix.size(); ++i)
	{
		for (size_t j = 0; j < outputMatrix[i].size(); ++j)
		{
			temp.push_back(outputMatrix[i][j]);
		}
	}

	std::vector<std::string> weights(temp.size());
	std::vector<std::pair<int, int>> edge(temp.size());

	for (size_t i = 0, x = 0, index = 0; i < temp.size(); ++i, ++index)
	{
		if (i % k == 0 && i != 0)
		{
			++x;
			index = 0;
		}
		weights[i] = 'x' + std::to_string(x) + 'y' + std::to_string(temp[i].second);
		edge[i] = std::make_pair(index, temp[i].first);
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), k);

	boost::dynamic_properties dp;
	dp.property("label", boost::get(boost::edge_weight, graph));
	dp.property("node_id", boost::get(boost::vertex_index, graph));

	write_graphviz_dp(fout, graph, dp);
}
