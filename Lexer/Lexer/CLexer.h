#pragma once
#include "Token.h"
#include "CommentState.h"
#include <vector>
#include <sstream>
#include <fstream>

class CLexer
{
public:
	CLexer(std::istream& input);

	void StartProcess();
	void PrintTokens(std::ostream& output);

private:
	void InitTokens();
	void InitKeywords();
	void InitOperations();
	void InitBrackets();
	void InitSeparators();
	void InitBinary();
	void InitOctal();
	void InitDecimal();
	void InitDecimalFractional();
	void InitHexadecimal();
	void InitComments();
	void InitComparisonsOperator();
	void InitLogicOperation();

	size_t CheckOperation(const std::string str);

	Token GetToken(const std::string line, std::string &str);
	bool HaveStringInVector(const std::vector<std::string> vstr, const std::string str);
	bool HaveCharInVector(const std::vector<char> vchar, const char ch);
	int GetPositionInLine(const std::string line, const std::string str);

	bool IsKeyword(const std::string str);
	bool IsComment(const std::string str);
	bool IsArithmeticOperation(const char ch);
	bool IsComparisonsOperator(const std::string str);
	bool IsLogicOperation(const std::string str);
	bool IsBracket(const char ch);
	bool IsSeparator(const char ch);
	bool IsBinary(const char ch);
	bool IsOctal(const char ch);
	bool IsDecimal(const char ch);
	bool IsDecimalFractional(const char ch);
	bool IsHexadecimal(const char ch);
	bool IsLetter(const char symbol);
	bool IsDigit(const char symbol);

	Token CheckComment(const std::string str);
	Token CheckId(std::string& str);
	Token CheckNumber(std::string& str);
	Token GetNumberToken(std::string& str, TokenType type, std::string system);
	Token CheckString(std::string& str);
	Token CheckChar(std::string& str);

	std::string TokenTypeToString(const TokenType tokenType);

    std::istream& m_input;
	size_t m_lineNumber;
	std::vector<Token> m_tokenVector;

	CommentState m_currCommentState;
	TokenType m_prevTokenType;
};

