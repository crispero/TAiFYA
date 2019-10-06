#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>

using matrix = std::vector<std::vector<std::pair<int, int>>>;
using vpair = std::vector<std::pair<int, int>>;

struct InputData {
	int x;
	int y;
	int s;
	std::string type;
	std::vector<int> Y;
	std::vector<std::vector<int>> mooreMatrix;
	vpair mealyMatrix;
};

struct SAndY {
	int s;
	int y;
};

int getNumberFromInput(std::ifstream& fin)
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

SAndY getTwoNumbersFromInput(std::ifstream& fin)
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

void ParseFromMoore(InputData &inputData, std::ifstream &fin)
{
	int i;
	do
	{
		inputData.Y.push_back(getNumberFromInput(fin));
	} while (fin.peek() != '\n');

	inputData.mooreMatrix.resize(inputData.x);
	for (int i = 0; i < inputData.x; i++)
	{
		inputData.mooreMatrix[i].resize(inputData.s);
		for (int j = 0; j < inputData.s; j++)
		{
			inputData.mooreMatrix[i][j] = getNumberFromInput(fin);
		}
	}
}

void ParseFromMealy(InputData& inputData, std::ifstream& fin)
{
	while (!fin.eof()) {
		SAndY dto = getTwoNumbersFromInput(fin);
		inputData.mealyMatrix.push_back(std::make_pair(dto.s, dto.y));
	}
}

InputData Parse(std::ifstream &fin)
{
	InputData inputData;
	fin >> inputData.x >> inputData.y >> inputData.s >> inputData.type;

	if (inputData.type == "moore")
	{
		ParseFromMoore(inputData, fin);
	}
	else if (inputData.type == "mealy")
	{
		ParseFromMealy(inputData, fin);
	}
	
	return inputData;
}

vpair getEdge(const InputData inputData)
{
	vpair edge;

	for (int i = 0; i < inputData.x; i++)
	{
		for (int j = 0; j < inputData.s; j++)
		{
			edge.push_back(std::make_pair(inputData.mooreMatrix[i][j], inputData.Y[inputData.mooreMatrix[i][j]]));
		}
	}

	return edge;
}

matrix getMealyMatrix(const InputData inputData)
{
	matrix matrix;
	matrix.resize(inputData.x);
	for (int i = 0; i < inputData.x; i++)
	{
		matrix[i].resize(inputData.s);
		for (int j = 0; j < inputData.s; j++)
		{
			matrix[i][j] = (std::make_pair(inputData.mooreMatrix[i][j], inputData.Y[inputData.mooreMatrix[i][j]]));
		}
	}

	return matrix;
}

void printMealyMatrix(const matrix matrix, int x, int s)
{
	std::ofstream fout("mealyMatrix.txt");
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < s; j++)
		{
			fout << 'q' + std::to_string(matrix[i][j].first) << 'y' + std::to_string(matrix[i][j].second) << " ";
		}
		fout << std::endl;
	}
}	

void mealyVisualization(vpair voutput, int statesCount)
{
	using Edge = std::pair<int, int>;
	using Graph = boost::adjacency_list< boost::vecS,
		boost::vecS, boost::directedS,
		boost::property<boost::vertex_color_t,
		boost::default_color_type>,
		boost::property< boost::edge_weight_t, std::string>
	>;
	std::ofstream fout("mealyGraph.dot");
	std::vector<std::string> weights(voutput.size());
	vpair edge(voutput.size());

	for (int i = 0, x = 1, index = 0; i < voutput.size(); ++i, ++index)
	{
		if (i % statesCount == 0 && i != 0)
		{
			++x;
			index = 0;
		}
		weights[i] = 'x' + std::to_string(x) + '/' + 'y' + std::to_string(voutput[i].second);
		edge[i] = std::make_pair(index, voutput[i].first);
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), statesCount);

	boost::dynamic_properties dp;
	dp.property("label", boost::get(boost::edge_weight, graph));
	dp.property("node_id", boost::get(boost::vertex_index, graph));

	write_graphviz_dp(fout, graph, dp);
}

vpair getUnique(const InputData inputData)
{
	vpair unique;
	unique.resize(inputData.mealyMatrix.size());
	std::copy(inputData.mealyMatrix.begin(), inputData.mealyMatrix.end(), unique.begin());
	std::sort(unique.begin(), unique.end());
	unique.erase(std::unique(unique.begin(), unique.end()), unique.end());
	return unique;
}

std::vector<int> getIndexes(const vpair unique, const vpair mealyMatrix, int x, int s)
{
	std::vector<int> indexes;
	indexes.resize(unique.size() * x);
	for (int i = 0; i < unique.size(); ++i)
	{
		size_t index = i;
		int k = unique[i].first;
		for (int j = 0; j < x; ++j)
		{
			auto it = std::find(unique.begin(), unique.end(), mealyMatrix[k]);
			indexes[index] = (std::distance(unique.begin(), it));
			if (j < x - 1)
			{
				index += unique.size();
				k += s;
			}
		}
	}

	return indexes;
}

void printMooreMatrix(std::vector<int> indexes, vpair unique)
{
	std::ofstream fout("mooreMatrix.txt");
	for (size_t i = 0; i < indexes.size(); ++i)
	{	
		if ((i % unique.size() == 0) && i != 0)
		{
			fout << std::endl;
		}
		fout << "z" << indexes[i] << " ";
	}
}

void mooreVisualization(vpair unique, std::vector<int> indexes)
{
	using Edge = std::pair<int, int>;
	using Graph = boost::adjacency_list< boost::vecS,
		boost::vecS, boost::directedS,
		boost::property<boost::vertex_color_t,
		boost::default_color_type>,
		boost::property< boost::edge_weight_t, std::string>
	>;
	std::vector<std::string> weights(indexes.size());
	vpair edge(indexes.size());
	std::ofstream fout("mooreGraph.dot");

	for (int i = 0, x = 1, index = 0; i < indexes.size(); ++i, ++index)
	{
		if (i % unique.size() == 0 && i != 0)
		{
			++x;
			index = 0;
		}
		weights[i] = 'x' + std::to_string(x);
		edge[i] = { index, indexes[i] };
	}

	Graph graph(edge.begin(), edge.end(), weights.begin(), unique.size());

	boost::dynamic_properties dp;
	dp.property("label", boost::get(boost::edge_weight, graph));
	dp.property("node_id", boost::get(boost::vertex_index, graph));

	write_graphviz_dp(fout, graph, dp);
}

int main(int argc, char* argv[])
{
	std::string fileName = argv[1];
	std::ifstream fin(fileName);

	if (!fin.is_open())
	{
		std::cerr << "Failed to open " << fileName << " for reading\n";
		return 1;
	}

	InputData inputData = Parse(fin);
	
	if (inputData.type == "moore")
	{
		vpair edge = getEdge(inputData);
		matrix mealyMatrix = getMealyMatrix(inputData);

		printMealyMatrix(mealyMatrix, inputData.x, inputData.s);
		mealyVisualization(edge, inputData.s);
	}
	else if (inputData.type == "mealy")
	{
		vpair unique = getUnique(inputData);
		std::vector<int> indexes = getIndexes(unique, inputData.mealyMatrix, inputData.x, inputData.s);
		
		printMooreMatrix(indexes, unique);
		mooreVisualization(unique, indexes);
	}
	else
	{
		return 1;
	}
	

	return 0;
}

