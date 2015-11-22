#include "stdafx.h"
#include "Lexer.h"

Lexer::~Lexer()
{

}

std::vector<Token> Lexer::Do()
{
	ptrdiff_t lineNumber = 1, lineStart = -1;
	std::vector<Token> tokens;
	while (index < length)
	{
		char current = data[index];
		if (isBlank(current))
		{
			if (current == '\n')
			{
				++lineNumber;
				lineStart = index;
			}
			// skip
		}
		else if (isIdFirst(current))
		{
			std::string word;
			word += current;
			while (nextIs(isId))
			{
				++index;
				word += data[index];
			}

			TokenType type;

			if ((type = reservedWordType(word)) == TOKENTYPE_NOTTOKEN)
			{
				Token token(TOKENTYPE_ID, word, lineNumber, index - lineStart);
				token.LexerID = Ids.GetID(word);
				tokens.push_back(token);
			}
			else
			{
				tokens.push_back(Token(type, word, lineNumber, index - lineStart));
			}
		}
		else if (isDigitFirst(current))
		{
			bool hasDot = false;
			std::string number;
			number += current;
			if (current == '.')
			{
				hasDot = true;
			}
			while (nextIs(isDigit) || (!hasDot && nextIs('.')))
			{
				++index;
				number += data[index];
				if (data[index] == '.')
				{
					hasDot = true;
				}
			}
			if (!hasDot)
			{
				Token token(TOKENTYPE_INTNUMBER, number, lineNumber, index - lineStart);
				token.IntValue = StringHelper_toInt(number);
				tokens.push_back(token);
			}
			else
			{
				if (number == ".") // only a dot
				{
					Token token(TOKENTYPE_OPDOT, number, lineNumber, index - lineStart);
					tokens.push_back(token);
				}
				else
				{
					Token token(TOKENTYPE_FLOATNUMBER, number, lineNumber, index - lineStart);
					token.FloatValue = StringHelper_toDouble(number);
					tokens.push_back(token);
				}
			}
		}
		else if (current == '(')
		{
			tokens.push_back(Token(TOKENTYPE_LBRACKET, "(", lineNumber, index - lineStart));
		}
		else if (current == ')')
		{
			tokens.push_back(Token(TOKENTYPE_RBRACKET, ")", lineNumber, index - lineStart));
		}
		else if (current == '!')
		{
			tokens.push_back(Token(TOKENTYPE_OPNOT, "!", lineNumber, index - lineStart));
		}
		else if (current == '&')
		{
			if (nextIs('&'))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPAND, "&&", lineNumber, index - lineStart));
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPBITAND, "&", lineNumber, index - lineStart));
			}
		}
		else if (current == '|')
		{
			if (nextIs('|'))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPOR, "||", lineNumber, index - lineStart));
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPBITOR, "|", lineNumber, index - lineStart));
			}
		}
		else if (current == '^')
		{
			if (nextIs('^'))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPXOR, "^^", lineNumber, index - lineStart));
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPBITXOR, "^", lineNumber, index - lineStart));
			}
		}
		else if (current == '~')
		{
			tokens.push_back(Token(TOKENTYPE_OPBITNOT, "~", lineNumber, index - lineStart));
		}
		else if (current == '-')
		{
			if (nextIs('>'))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPIMP, "->", lineNumber, index - lineStart));
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPSUB, "-", lineNumber, index - lineStart));
			}
		}
		else if (current == '<')
		{
			if (nextIs('='))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPLTE, "<=", lineNumber, index - lineStart));
			}
			else if (nextIs('-'))
			{
				++index;
				if (nextIs('>'))
				{
					++index;
					tokens.push_back(Token(TOKENTYPE_OPDUALIMP, "<->", lineNumber, index - lineStart));
				}
				else
				{
					throw SyntaxError("Lexer: Unexpected " + nextChar() + ", expecting '>'");
				}
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPLT, "<", lineNumber, index - lineStart));
			}
		}
		else if (current == '=')
		{
			if (nextIs('>'))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPTAUIMP, "=>", lineNumber, index - lineStart));
			}
			else if(nextIs('='))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPEQU, "==", lineNumber, index - lineStart));
			}
			else
			{
				throw SyntaxError("Lexer: Line " + StringHelper_toString(lineNumber) + ", Col " + StringHelper_toString(index - lineStart + 1) + ": Unexpected " + nextChar() + ", expecting '>', '='");
			}
		}
		else if (current == '>')
		{
			if (nextIs('='))
			{
				++index;
				tokens.push_back(Token(TOKENTYPE_OPGTE, ">=", lineNumber, index - lineStart));
			}
			else
			{
				tokens.push_back(Token(TOKENTYPE_OPGT, ">", lineNumber, index - lineStart));
			}
		}
		else if (current == '+')
		{
			tokens.push_back(Token(TOKENTYPE_OPADD, "+", lineNumber, index - lineStart));
		}
		else if (current == '*')
		{
			tokens.push_back(Token(TOKENTYPE_OPMUL, "*", lineNumber, index - lineStart));
		}
		else if (current == '/')
		{
			tokens.push_back(Token(TOKENTYPE_OPDIV, "/", lineNumber, index - lineStart));
		}
		else if (current == '%')
		{
			tokens.push_back(Token(TOKENTYPE_OPMOD, "%", lineNumber, index - lineStart));
		}
		else
		{
			throw SyntaxError("Lexer: Line " + StringHelper_toString(lineNumber) + ", Col " + StringHelper_toString(index - lineStart) + ": Unexpected '" + current + "', expecting A-Z, a-z, 0-9, '.', '$', '_', '(', ')', '!', '&', '|', '^', '~', '-', '<', '>', '=', '+', '*', '/', '%'");
		}
		++index;
	}
	return std::move(tokens);
}

bool Lexer::hasNext()
{
	return index < length - 1;
}

char Lexer::getNext()
{
	return data[index + 1];
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

bool Lexer::nextIs(bool (*cond)(char))
{
	return hasNext() && cond(data[index + 1]);
}

TokenType Lexer::reservedWordType(std::string &str)
{
	if (str == "wandai")
	{
		return TOKENTYPE_WANDAI;
	}
	else if (str == "true")
	{
		return TOKENTYPE_TRUE;
	}
	else if (str == "false")
	{
		return TOKENTYPE_FALSE;
	}
	else
	{
		return TOKENTYPE_NOTTOKEN;
	}
}

bool Lexer::isBlank(char a)
{
	return a == ' ' || a == '\n' || a == '\r' || a == '\t';
}

bool Lexer::isDigitFirst(char a)
{
	return (a >= '0' && a <= '9') || a == '.';
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