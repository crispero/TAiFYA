#pragma once
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <iostream>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>

class IMachineMinimization
{
public:
	IMachineMinimization() = default;
	virtual ~IMachineMinimization() = default;
	virtual void Parse(std::istream& fin) = 0;
	virtual void Minimization() = 0;
	virtual void Print(std::ostream& fout) const = 0;
	virtual void Visualization() const = 0;
};