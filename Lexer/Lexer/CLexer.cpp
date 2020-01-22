#include "CLexer.h"
#include <algorithm>

namespace Lexer
{
	std::vector<std::string> KEYWORDS, COMMENTS, COMPARISSONS_OPERATOR, LOGICS_OPERATOR;
	std::vector<char> OPERATIONS, BRACKETS, SEPARATORS, BINARY, OCTAL, DECIMAL, HEXADECIMAL, DECIMAL_FRACTIONAL;
}

CLexer::CLexer(std::istream& input)
	: m_input(input)
	, m_currCommentState(CommentState::NO_COMMENT)
{
	m_lineNumber = 0;
	m_currCommentState = CommentState::NO_COMMENT;
	m_prevTokenType = TokenType::Id;
}

Token CLexer::GetToken(const std::string line, std::string& str)
{
	Token token = CheckComment(str);
	if (token.type != TokenType::Comment && m_currCommentState == CommentState::NO_COMMENT)
	{
		auto length = CheckOperation(str);
		bool isCheckOperation = length > 0;
		size_t lenghtWithBracket = 0;
		std::string substr;

		if (!isCheckOperation)
		{
			size_t foundPosition = str.find("(");
			if (foundPosition != std::string::npos && foundPosition != 0)
			{
				substr = str.substr(0, foundPosition);
				lenghtWithBracket = CheckOperation(substr);
			}
			if (foundPosition == 0)
			{
				substr = str[0];
				str.erase(0, 1);
				return Token{ TokenType::Bracket, substr, m_lineNumber, (size_t)GetPositionInLine(line, str) };
			}
		}

		if (isCheckOperation || lenghtWithBracket > 0)
		{
			std::string str1 = str;
			str.erase(0, isCheckOperation ? length : lenghtWithBracket);
			return Token{ TokenType::Keyword, isCheckOperation ? str1 : substr, m_lineNumber, (size_t)GetPositionInLine(line, str) };
		}

		if (IsArithmeticOperation(str[0]) && !IsDigit(str[1]))
		{
			substr = str.substr(0, 1);
			str.erase(0, 1);
			return Token{ TokenType::ArithmeticOperation, substr, m_lineNumber, (size_t)GetPositionInLine(line, str) };
		}

		if (IsLogicOperation(str))
		{
			std::string str1 = str;
			str.erase(str.begin(), str.end());
			return Token{ TokenType::LogicOperator, str1, m_lineNumber, (size_t)GetPositionInLine(line, str) };
		}

		if (IsComparisonsOperator(str))
		{
			std::string str1 = str;
			str.erase(str.begin(), str.end());
			return Token{ TokenType::ComparisonsOperator, str1, m_lineNumber, (size_t)GetPositionInLine(line, str) };
		}

		if (IsBracket(str[0]))
		{
			substr = str.substr(0, 1);
			str.erase(0, 1);
			return Token{ TokenType::Bracket, substr, m_lineNumber, (size_t)GetPositionInLine(line, str) };
		}

		if (IsSeparator(str[0]))
		{
			substr = str.substr(0, 1);
			str.erase(0, 1);
			return Token{ TokenType::Separator, substr, m_lineNumber, (size_t)GetPositionInLine(line, str) };
		}

		if (str[0] == '[')
		{
			std::string strArr;
			strArr.push_back(str[0]);
			for (int i = 0; i < str.size(); i++)
			{
				if (str[i] == ']')
				{
					strArr.push_back(str[i]);
					str.erase(i);
					break;
				}
			}

			str.erase(0, 1);
			return Token{ TokenType::Array, strArr, m_lineNumber };
		}

		if (str[0] == '"')
		{
			return CheckString(str);
		}

		if (str[0] == '\'')
		{
			return CheckChar(str);
		}

		if (IsLetter(str[0]))
		{
			return CheckId(str);
		}

		if (IsDigit(str[0]))
		{
			return CheckNumber(str);
		}
	}
	else
	{
		std::string substr = str;
		token.value = substr;
		str.erase(str.begin(), str.end());
		token.lineNumber = m_lineNumber;
		return token;
	}

	std::string str1 = str;
	str.erase(str.begin(), str.end());
	return Token{ TokenType::Error, str1, m_lineNumber };
}

size_t CLexer::CheckOperation(const std::string str)
{
	auto it = std::find(Lexer::KEYWORDS.begin(), Lexer::KEYWORDS.end(), str);
	return it != Lexer::KEYWORDS.end() ? (*it).length() : 0;
}

bool CLexer::IsKeyword(const std::string str)
{
	return HaveStringInVector(Lexer::KEYWORDS, str);
}

bool CLexer::IsComment(const std::string str)
{
	return HaveStringInVector(Lexer::COMMENTS, str);
}

bool CLexer::IsArithmeticOperation(const char ch)
{
	return HaveCharInVector(Lexer::OPERATIONS, ch);
}

bool CLexer::IsComparisonsOperator(const std::string str)
{
	return HaveStringInVector(Lexer::COMPARISSONS_OPERATOR, str);
}

bool CLexer::IsLogicOperation(const std::string str)
{
	return HaveStringInVector(Lexer::LOGICS_OPERATOR, str);
}

bool CLexer::IsBracket(const char ch)
{
	return HaveCharInVector(Lexer::BRACKETS, ch);
}

bool CLexer::IsSeparator(const char ch)
{
	return HaveCharInVector(Lexer::SEPARATORS, ch);
}

bool CLexer::IsBinary(const char ch)
{
	return HaveCharInVector(Lexer::BINARY, ch);
}

bool CLexer::IsOctal(const char ch)
{
	return HaveCharInVector(Lexer::OCTAL, ch);
}

bool CLexer::IsDecimal(const char ch)
{
	return HaveCharInVector(Lexer::DECIMAL, ch);
}	

bool CLexer::IsHexadecimal(const char ch)
{
	return HaveCharInVector(Lexer::HEXADECIMAL, ch);
}

bool CLexer::IsDecimalFractional(const char ch)
{
	return HaveCharInVector(Lexer::DECIMAL_FRACTIONAL, ch);
}

Token CLexer::CheckComment(const std::string str)
{
	Token token;
	std::string tempStr;

	for (int i = 0; i < str.length(); i++)
	{
		char currCh = str[i];
		char nextCh = str[i + 1];

		if ((currCh == '/' || currCh == '*') && (nextCh == '*' || nextCh == '/'))
		{
			tempStr.push_back(currCh);
			tempStr.push_back(nextCh);
			break;
		}
	}

	if (IsComment(tempStr) || m_currCommentState != CommentState::NO_COMMENT)
	{

		if (tempStr == "//" && m_currCommentState != CommentState::MULTILINE)
		{
			m_currCommentState = CommentState::SINGLE_LINE;
		}
		else if (tempStr == "/*")
		{
			m_currCommentState = CommentState::MULTILINE;
		}
		else if (tempStr == "*/")
		{
			m_currCommentState = CommentState::NO_COMMENT;
		}

		if (m_currCommentState == CommentState::SINGLE_LINE)
		{
			m_currCommentState = CommentState::NO_COMMENT;
		}

		token.type = TokenType::Comment;
	}

	return token;
}

Token CLexer::CheckId(std::string& str)
{
	std::string id;
	for (char ch : str)
	{
		if (IsLetter(ch) || IsDigit(ch))
		{
			id.push_back(ch);
		}
	}

	str.erase(0, id.size());
	return Token{ TokenType::Id, id, m_lineNumber };
}

Token CLexer::CheckNumber(std::string& str)
{
	Token token;
	std::string system = str.substr(0, 2);
	std::string substr = str.length() >= 2 ? str.substr(2) : str;
	std::string digits;

	if (system == "0b")
	{
		token = GetNumberToken(substr, TokenType::Binary, system);
	}
	else if (system == "0o")
	{
		token = GetNumberToken(substr, TokenType::Octal, system);
	}
	else if (system == "0x")
	{
		token = GetNumberToken(substr, TokenType::Hexadecimal, system);
	}
	else
	{
		for (int i = 0; i < str.length(); i++)
		{
			char currCh = str[i];
			char nextCh = str[i + 1];
			bool isNextFloat = (IsDigit(nextCh) || nextCh == '+' || nextCh == '-');
			bool isDecimalFractional = IsDecimalFractional(currCh);

			if (!IsSeparator(currCh) && (IsDigit(currCh) || isDecimalFractional || currCh == '+' || currCh == '-'))
			{
				if (isDecimalFractional && isNextFloat)
				{
					token.type = TokenType::Float;
				}

				if (token.type != TokenType::Float)
				{
					token.type = isDecimalFractional && !isNextFloat ? TokenType::Error : TokenType::Integer;
				}
				digits.push_back(currCh);
				token.value = digits;
			}
			else if (IsLetter(currCh))
			{
				token.type = TokenType::Error;
				break;
			}
		}
	}

	if (token.type != TokenType::Error)
	{
		str.erase(0, token.value.size());
	}
	else
	{	
		std::string temp;
		for (char ch : str)
		{
			if (!IsSeparator(ch))
			{
				temp.push_back(ch);
			}
		}
		token.value = temp;
		str.erase(0, temp.size());
	}
	
	return token;
}

Token CLexer::GetNumberToken(std::string& str, TokenType type, std::string system)
{
	Token token;
	std::string digits = system;
	for (char ch : str)
	{
		if (!IsSeparator(ch))
		{
			if (type == TokenType::Binary ? IsBinary(ch) : type == TokenType::Octal ? IsOctal(ch) : IsHexadecimal(ch))
			{
				token.type = type;
			}
			else
			{
				token.type = TokenType::Error;
				break;
			}
			digits.push_back(ch);
		}
	}

	token.value = digits;
	token.lineNumber = m_lineNumber;

	return token;
}

Token CLexer::CheckString(std::string& str)
{
	std::string temp;
	for (char ch : str)
	{
		if (!IsSeparator(ch))
		{
			temp.push_back(ch);
		}
	}

	if (temp[temp.size() - 1] == '"')
	{
		str.erase(0, temp.size());
		return Token{ TokenType::String, temp, m_lineNumber };
	}
	else
	{
		str.erase(0, temp.size());
		return Token{ TokenType::Error, temp, m_lineNumber };
	}
}

Token CLexer::CheckChar(std::string& str)
{
	std::string temp;
	for (char ch : str)
	{
		if (!IsSeparator(ch))
		{
			temp.push_back(ch);
		}
	}

	if (temp[temp.size() - 1] == '\'')
	{
		str.erase(0, temp.size());
		return Token{ TokenType::Char, temp, m_lineNumber };
	}
	else
	{
		str.erase(0, temp.size());
		return Token{ TokenType::Error, temp, m_lineNumber };
	}
}

bool CLexer::HaveStringInVector(const std::vector<std::string> vstr, const std::string str)
{
	return std::find(vstr.begin(), vstr.end(), str) != vstr.end();
}

bool CLexer::HaveCharInVector(const std::vector<char> vchar, const char ch)
{
	return std::find(vchar.begin(), vchar.end(), ch) != vchar.end();
}

int CLexer::GetPositionInLine(const std::string line, const std::string str)
{
	return line.find(str);
}

bool CLexer::IsDigit(const char symbol)
{
	return isdigit(symbol);
}

bool CLexer::IsLetter(const char symbol)
{
	return (isalpha(symbol) || (symbol == '_'));
}

void CLexer::InitTokens()
{
	InitKeywords();
	InitOperations();
	InitBrackets();
	InitSeparators();
	InitBinary();
	InitOctal();
	InitDecimal();
	InitDecimalFractional();
	InitHexadecimal();
	InitComments();
	InitComparisonsOperator();
	InitLogicOperation();
}

void CLexer::InitKeywords()
{
	Lexer::KEYWORDS = { "if", "else", "while", "for", "read", "write", "return", "and", "or", "int", "void", "main", "string", "char", "float" };
}

void CLexer::InitOperations()
{
	Lexer::OPERATIONS = {'+', '-', '*', '/', '='};
}

void CLexer::InitComparisonsOperator()
{
	Lexer::COMPARISSONS_OPERATOR = { "==", "!=", ">", "<", ">=", "<=" };
}

void CLexer::InitLogicOperation()
{
	Lexer::LOGICS_OPERATOR = { "&&", "||", "!" };
}

void CLexer::InitBrackets()
{
	Lexer::BRACKETS = { '{', '}', '(', ')' };
}

void CLexer::InitSeparators()
{
	Lexer::SEPARATORS = { ':', ',', ';' };
}

void CLexer::InitComments()
{
	Lexer::COMMENTS = { "//", "/*", "*/" };
}

void CLexer::InitBinary()
{
	Lexer::BINARY = { '0', '1' };
}

void CLexer::InitOctal()
{
	Lexer::OCTAL = { '0', '1', '2', '3', '4', '5', '6', '7' };
}

void CLexer::InitDecimal()
{
	Lexer::DECIMAL = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
}

void CLexer::InitDecimalFractional()
{
	Lexer::DECIMAL_FRACTIONAL = { 'e', 'E', '.' };
}

void CLexer::InitHexadecimal()
{
	Lexer::HEXADECIMAL = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
}

void CLexer::StartProcess()
{
	InitTokens();
	std::string line;
	while (getline(m_input, line))
	{
		++m_lineNumber;
		std::istringstream iss(line);
		std::string str;

		while (iss >> str)
		{
			while (!str.empty())
			{
				size_t positionIndex = line.find(str);
				Token token = GetToken(line, str);
				token.positionIndex = positionIndex;
				if (token.type != TokenType::Comment)
				{
					m_tokenVector.push_back(token);
				}
			}
		}
	}
}

void CLexer::PrintTokens(std::ostream& output)
{
	for (Token token : m_tokenVector)
	{
		output << TokenTypeToString(token.type) <<" " << token.value << " " << token.lineNumber << " " << token.positionIndex << "\n";
	}
}

std::string CLexer::TokenTypeToString(const TokenType tokenType)
{
	if (tokenType == TokenType::ArithmeticOperation) return "Arithmetic operation";
	if (tokenType == TokenType::Array) return "Array";
	if (tokenType == TokenType::Binary) return "Binary";
	if (tokenType == TokenType::Bracket) return "Bracket";
	if (tokenType == TokenType::Comment) return "Comment";
	if (tokenType == TokenType::Decimal) return "Decimal";
	if (tokenType == TokenType::DecimalFractional) return "DecimalFractional";
	if (tokenType == TokenType::Error) return "Error";
	if (tokenType == TokenType::Float) return "Float";
	if (tokenType == TokenType::Hexadecimal) return "Hexadecimal";
	if (tokenType == TokenType::Id) return "Id";
	if (tokenType == TokenType::Integer) return "Integer";
	if (tokenType == TokenType::Keyword) return "Keyword";
	if (tokenType == TokenType::Octal) return "Octal";
	if (tokenType == TokenType::Separator) return "Separator";
	if (tokenType == TokenType::String) return "String";
	if (tokenType == TokenType::Char) return "Char";

	return "";
}
