#pragma once

#include <vector>
#include <string>
#include "SyntaxError.h"
#include "Token.h"

class Lexer
{
public:
	Lexer(std::string& str) : data(str)
	{
		length = str.length();
	}

	~Lexer();
	std::vector<Token> Do();

private:
	std::string& data;
	int length;
	int index = 0;

	bool hasNext();
	std::string nextChar();
	bool nextIs(char);
	bool isReserved(std::string);
	bool isBlank(char);
	bool isDigit(char);
	bool isLetter(char);
	bool isUppercase(char);
	bool isLowercase(char);
};