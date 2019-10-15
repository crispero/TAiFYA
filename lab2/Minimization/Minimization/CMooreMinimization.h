#pragma once
#include "IMachineMinimization.h"

class CMooreMinimization : public IMachineMinimization
{
public:
	CMooreMinimization(const size_t x, const size_t y, const size_t s);
	~CMooreMinimization() = default;

	void Parse(std::istream& fin) override;
	void Minimization() override;
	void Print(std::ostream& fout) const override;
	void Visualization() const override;
private:
	int GetNumberFromInput(std::istream& fin);
	std::vector<int> GetUnique(const std::vector<int>& arr);
	std::vector<int> GetFinalOutputs(const std::vector<int>& temp);
	std::map<int, std::vector<std::pair<int, std::vector<int>>>> GetDontKnowWhatKindOfMap();
	std::map<int, std::vector<int>> GetTempMapForMinimization(const std::map<int, std::vector<std::pair<int, std::vector<int>>>> & dontKnowWhatKindOfMap);
	void PrepareToPrint();
	void SetMapForMinimization();
	void SetVisualizationVector();

	const size_t m_x;
	const size_t m_y;
	const size_t m_s;
	std::map<int, std::vector<int>> m_mapForMinimization;
	std::vector<int> m_outputAlphabetCharacters;
	std::vector<std::vector<int>> m_inputMatrix;
	std::vector<int> m_finalOutputs;
	std::vector<std::vector<int>> m_outputMatrix;
	std::vector<int> m_visualizationVector;
};

