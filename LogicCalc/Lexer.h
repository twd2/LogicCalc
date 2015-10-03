#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include "SyntaxError.h"
#include "Token.h"

class Lexer
{
public:
	Lexer(std::string &str) : data(str)
	{
		length = str.length();
		reservedWords.push_back("wandai");
	}

	~Lexer();
	std::vector<Token> Do();
	std::vector<std::string> Symbols;

private:
	std::string &data;

	std::vector<std::string> reservedWords;

	int length;
	int index = 0;

	bool hasNext();
	std::string nextChar();
	bool nextIs(char);
	bool isReserved(std::string&);
	bool isBlank(char);
	bool isIdFirst(char);
	bool isId(char);
	bool isDigit(char);
	bool isLetter(char);
	bool isUppercase(char);
	bool isLowercase(char);
};