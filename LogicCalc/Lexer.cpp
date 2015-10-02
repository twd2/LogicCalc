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
		else if (isIdFirst(current))
		{
			std::string word;
			word += current;
			while (hasNext() && isId(data[index + 1]))
			{
				++index;
				word += data[index];
			}
			if (!isReserved(word))
			{
				tokens.push_back(Token(TOKENTYPE_ID, word));
			}
			else
			{
				if (word == "wandai")
				{
					tokens.push_back(Token(TOKENTYPE_WANDAI, word));
				}
				else
				{
					//TODO: other reserved words
				}
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
			tokens.push_back(Token(TOKENTYPE_NUMBER, number));
		}
		else if (current == '(')
		{
			tokens.push_back(Token(TOKENTYPE_LBRACKET, "("));
		}
		else if (current == ')')
		{
			tokens.push_back(Token(TOKENTYPE_RBRACKET, ")"));
		}
		else if (current == '!')
		{
			tokens.push_back(Token(TOKENTYPE_OPNOT, "!"));
		}
		else if (current == '&')
		{
			if (nextIs('&'))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPAND, "&&"));
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPBITAND, "&"));
			}
		}
		else if (current == '|')
		{
			if (nextIs('|'))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPOR, "||"));
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPBITOR, "|"));
			}
		}
		else if (current == '^')
		{
			if (nextIs('^'))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPXOR, "^^"));
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPBITXOR, "^"));
			}
		}
		else if (current == '~')
		{
			tokens.push_back(Token(TOKENTYPE_OPBITNOT, "~"));
		}
		else if (current == '-')
		{
			if (nextIs('>'))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPIMP, "->"));
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPSUB, "-"));
			}
		}
		else if (current == '<')
		{
			if (nextIs('='))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPLTE, "<="));
			}
			else if (nextIs('-'))
			{
				++index;
				if (nextIs('>'))
				{
					++index;
					tokens.push_back(Token(TOKENTYPE_OPDUALIMP, "<->"));
				}
				else
				{
					throw SyntaxError("unexpected " + nextChar() + ", expecting '>'");
				}
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPLT, "<"));
			}
		}
		else if (current == '=')
		{
			if (nextIs('>'))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPTAUIMP, "=>"));
			}
			else if(nextIs('='))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPEQU, "=="));
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
				tokens.push_back(Token(TOKENTYPE_OPGTE, ">="));
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPGT, ">"));
			}
		}
		else if (current == '+')
		{
			tokens.push_back(Token(TOKENTYPE_OPADD, "+"));
		}
		else if (current == '*')
		{
			tokens.push_back(Token(TOKENTYPE_OPMUL, "*"));
		}
		else if (current == '/')
		{
			tokens.push_back(Token(TOKENTYPE_OPDIV, "/"));
		}
		else if (current == '%')
		{
			tokens.push_back(Token(TOKENTYPE_OPMOD, "%"));
		}
		else
		{
			throw SyntaxError();
		}
		++index;
	}
	return std::move(tokens);
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
	auto iter = std::find(reservedWords.begin(), reservedWords.end(), str);
	return iter != reservedWords.end();
}

bool Lexer::isBlank(char a)
{
	return a == ' ' || a == '\n' || a == '\r' || a == '\t';
}

bool Lexer::isDigit(char a)
{
	return a >= '0' && a <= '9';
}

bool Lexer::isIdFirst(char a)
{
	return a == '_' || a == '$' || isLetter(a);
}

bool Lexer::isId(char a)
{
	return isIdFirst(a) || isDigit(a);
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