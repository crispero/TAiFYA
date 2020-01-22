#pragma once
#include "TokenType.h"
#include <string>

struct Token
{
	TokenType type;
	std::string value;
	size_t lineNumber = 1;
	size_t positionIndex = 0;
};