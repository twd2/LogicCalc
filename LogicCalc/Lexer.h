#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include "SyntaxError.h"
#include "Token.h"
#include "IdGenerator.h"

class Lexer
{
public:
	Lexer(std::string &str) : data(str)
	{
		length = str.length();
	}

	~Lexer();
	std::vector<Token> Do();
	IdGenerator Ids;

	static TokenType Lexer::reservedWordType(std::string&);
	static bool isBlank(char);
	static bool isIdFirst(char);
	static bool isId(char);
	static bool isDigitFirst(char);
	static bool isDigit(char);
	static bool isLetter(char);
	static bool isUppercase(char);
	static bool isLowercase(char);

private:
	std::string &data;

	size_t length;
	size_t index = 0;

	bool hasNext();
	char getNext();
	std::string nextChar();
	bool nextIs(char);
	bool nextIs(bool(*)(char));
};