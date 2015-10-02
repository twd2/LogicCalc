#include "stdafx.h"
#include "Lexer.h"

Lexer::~Lexer()
{

}

std::vector<Token> Lexer::Do()
{
	int lineNumber = 1;
	std::vector<Token> tokens;
	while (index < length)
	{
		char current = data[index];
		if (isBlank(current))
		{
			if (current == '\n') ++lineNumber;
			//skip
		}
		else if (isLetter(current))
		{
			std::string word;
			word += current;
			while (hasNext() && isLetter(data[index + 1]))
			{
				++index;
				word += data[index];
			}
			if (!isReserved(word))
			{
				tokens.push_back(Token(ID, word));
			}
			else
			{
				//TODO: reserved word
			}
		}
		else if (isDigit(current))
		{
			std::string number;
			number += current;
			while (hasNext() && isDigit(data[index + 1]))
			{
				++index;
				number += data[index];
			}
			tokens.push_back(Token(NUMBER, number));
		}
		else if (current == '(')
		{
			tokens.push_back(Token(LBRACKET, "("));
		}
		else if (current == ')')
		{
			tokens.push_back(Token(RBRACKET, ")"));
		}
		else if (current == '!')
		{
			tokens.push_back(Token(OPNOT, "!"));
		}
		else if (current == '&')
		{
			if (nextIs('&'))
			{
				++index;
				tokens.push_back(Token(OPAND, "&&"));
			}
			else
			{
				tokens.push_back(Token(OPBITAND, "&"));
			}
		}
		else if (current == '|')
		{
			if (nextIs('|'))
			{
				++index;
				tokens.push_back(Token(OPOR, "||"));
			}
			else
			{
				tokens.push_back(Token(OPBITOR, "|"));
			}
		}
		else if (current == '^')
		{
			if (nextIs('^'))
			{
				++index;
				tokens.push_back(Token(OPXOR, "^^"));
			}
			else
			{
				tokens.push_back(Token(OPBITXOR, "^"));
			}
		}
		else if (current == '~')
		{
			tokens.push_back(Token(OPBITNOT, "~"));
		}
		else if (current == '-')
		{
			if (nextIs('>'))
			{
				++index;
				tokens.push_back(Token(OPIMP, "->"));
			}
			else
			{
				tokens.push_back(Token(OPSUB, "-"));
			}
		}
		else if (current == '<')
		{
			if (nextIs('='))
			{
				++index;
				tokens.push_back(Token(OPLTE, "<="));
			}
			else if (nextIs('-'))
			{
				++index;
				if (nextIs('>'))
				{
					++index;
					tokens.push_back(Token(OPDUALIMP, "<->"));
				}
				else
				{
					throw SyntaxError("unexpected " + nextChar() + ", expecting '>'");
				}
			}
			else
			{
				tokens.push_back(Token(OPLT, "<"));
			}
		}
		else if (current == '=')
		{
			if (nextIs('>'))
			{
				++index;
				tokens.push_back(Token(OPTAUIMP, "=>"));
			}
			else if(nextIs('='))
			{
				++index;
				tokens.push_back(Token(OPEQU, "=="));
			}
			else
			{
				throw SyntaxError("unexpected " + nextChar() + ", expecting '>', '='");
			}
		}
		else if (current == '>')
		{
			if (nextIs('='))
			{
				++index;
				tokens.push_back(Token(OPGTE, ">="));
			}
			else
			{
				tokens.push_back(Token(OPGT, ">"));
			}
		}
		else if (current == '+')
		{
			tokens.push_back(Token(OPADD, "+"));
		}
		else if (current == '*')
		{
			tokens.push_back(Token(OPMUL, "*"));
		}
		else if (current == '/')
		{
			tokens.push_back(Token(OPDIV, "/"));
		}
		else if (current == '%')
		{
			tokens.push_back(Token(OPMOD, "%"));
		}
		else
		{
			throw SyntaxError();
		}
		++index;
	}
	return tokens;
}

bool Lexer::hasNext()
{
	return index < length - 1;
}

std::string Lexer::nextChar()
{
	if (!hasNext())
	{
		return "EOF";
	}
	else
	{
		return "'" + std::string(1, data[index + 1]) + "'";
	}
}

bool Lexer::nextIs(char what)
{
	return hasNext() && data[index + 1] == what;
}

bool Lexer::isReserved(std::string &str)
{
	return false;
}

bool Lexer::isBlank(char a)
{
	return a == ' ' || a == '\n' || a == '\r' || a == '\t';
}

bool Lexer::isDigit(char a)
{
	return a >= '0' && a <= '9';
}

bool Lexer::isLetter(char a)
{
	return (a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z');
}

bool Lexer::isUppercase(char a)
{
	return a >= 'A' && a <= 'Z';
}

bool Lexer::isLowercase(char a)
{
	return a >= 'a' && a <= 'z';
}