#include "stdafx.h"
#include "Lexer.h"

Lexer::~Lexer()
{

}

std::vector<Token> Lexer::Do(bool preserveComments)
{
	std::vector<Token> tokens;
	while (index < length)
	{
		tokenStart = index;
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
				Token token = MakeToken(TOKENTYPE_ID, word);
				token.LexerID = Ids.GetID(word);
				tokens.push_back(token);
			}
			else
			{
				tokens.push_back(MakeToken(type, word));
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
				Token token = MakeToken(TOKENTYPE_INTNUMBER, number);
				token.IntValue = StringHelper_toInt(number);
				tokens.push_back(token);
			}
			else
			{
				if (number == ".") // only a dot
				{
					tokens.push_back(MakeToken(TOKENTYPE_OPDOT, number));
				}
				else
				{
					Token token = MakeToken(TOKENTYPE_FLOATNUMBER, number);
					token.FloatValue = StringHelper_toDouble(number);
					tokens.push_back(token);
				}
			}
		}
		else if (current == ',')
		{
			tokens.push_back(MakeToken(TOKENTYPE_OPCOMMA, ","));
		}
		else if (current == ':')
		{
			tokens.push_back(MakeToken(TOKENTYPE_OPCOLON, ":"));
		}
		else if (current == ';')
		{
			tokens.push_back(MakeToken(TOKENTYPE_OPSEMICOLON, ";"));
		}
		else if (current == '\'')
		{
			std::string ch;
			ch += current;
			bool escape = false, ended = false, gotten = false;
			while (hasNext())
			{
				++index;
				ch += data[index];
				if (data[index] == '\\' && !escape)
				{
					escape = true;
				}
				else if (data[index] == '\'' && !escape)
				{
					ended = true;
					break;
				}
				else if (!gotten)
				{
					gotten = true;
					escape = false;
				}
				else // already gotten!
				{
					// more than one char!
					throw Error("'" + std::string(1, data[index]) + "'", "'\\''");
				}
			}
			
			if (!gotten && !ended)
			{
				throw Error("EOF", "a char");
			}
			else if (gotten && !ended)
			{
				throw Error("EOF", "'\\''");
			}
			else if (!gotten && ended)
			{
				throw Error("'\\''", "a char");
			}
			else if (gotten && ended)
			{
				// right!
			}

			tokens.push_back(MakeToken(TOKENTYPE_CHARVALUE, ch));
		}
		else if (current == '\"')
		{
			std::string str;
			str += current;
			bool escape = false, ended = false;
			while (hasNext())
			{
				++index;
				str += data[index];
				if (data[index] == '\\' && !escape)
				{
					escape = true;
				}
				else if (data[index] == '\"' && !escape)
				{
					ended = true;
					break;
				}
				else
				{
					escape = false;
				}
			}

			if (!ended)
			{
				throw Error("EOF", "'\\\"'");
			}

			tokens.push_back(MakeToken(TOKENTYPE_STRINGVALUE, str));
		}
		else if (current == '(')
		{
			tokens.push_back(MakeToken(TOKENTYPE_LROUNDBRACKET, "("));
		}
		else if (current == ')')
		{
			tokens.push_back(MakeToken(TOKENTYPE_RROUNDBRACKET, ")"));
		}
		else if (current == '[')
		{
			tokens.push_back(MakeToken(TOKENTYPE_LSQUAREBRACKET, "["));
		}
		else if (current == ']')
		{
			tokens.push_back(MakeToken(TOKENTYPE_RSQUAREBRACKET, "]"));
		}
		else if (current == '{')
		{
			tokens.push_back(MakeToken(TOKENTYPE_LBRACE, "{"));
		}
		else if (current == '}')
		{
			tokens.push_back(MakeToken(TOKENTYPE_RBRACE, "}"));
		}
		else if (current == '!')
		{
			if (nextIs('='))
			{
				++index;
				tokens.push_back(MakeToken(TOKENTYPE_OPNEQU, "!="));
			}
			else
			{
				tokens.push_back(MakeToken(TOKENTYPE_OPNOT, "!"));
			}
		}
		else if (current == '&')
		{
			if (nextIs('&'))
			{
				++index;
				tokens.push_back(MakeToken(TOKENTYPE_OPAND, "&&"));
			}
			else
			{
				tokens.push_back(MakeToken(TOKENTYPE_OPBITAND, "&"));
			}
		}
		else if (current == '|')
		{
			if (nextIs('|'))
			{
				++index;
				tokens.push_back(MakeToken(TOKENTYPE_OPOR, "||"));
			}
			else
			{
				tokens.push_back(MakeToken(TOKENTYPE_OPBITOR, "|"));
			}
		}
		else if (current == '^')
		{
			if (nextIs('^'))
			{
				++index;
				tokens.push_back(MakeToken(TOKENTYPE_OPXOR, "^^"));
			}
			else
			{
				tokens.push_back(MakeToken(TOKENTYPE_OPBITXOR, "^"));
			}
		}
		else if (current == '~')
		{
			tokens.push_back(MakeToken(TOKENTYPE_OPBITNOT, "~"));
		}
		else if (current == '-')
		{
			if (nextIs('>'))
			{
				++index;
				tokens.push_back(MakeToken(TOKENTYPE_OPIMP, "->"));
			}
			else
			{
				tokens.push_back(MakeToken(TOKENTYPE_OPSUB, "-"));
			}
		}
		else if (current == '<')
		{
			if (nextIs('='))
			{
				++index;
				tokens.push_back(MakeToken(TOKENTYPE_OPLTE, "<="));
			}
			else if (nextIs('-'))
			{
				++index;
				if (nextIs('>'))
				{
					++index;
					tokens.push_back(MakeToken(TOKENTYPE_OPDUALIMP, "<->"));
				}
				else
				{
					throw Error(nextChar(), "'>'");
				}
			}
			else
			{
				tokens.push_back(MakeToken(TOKENTYPE_OPLT, "<"));
			}
		}
		else if (current == '=')
		{
			if (nextIs('>'))
			{
				++index;
				tokens.push_back(MakeToken(TOKENTYPE_OPTAUIMP, "=>"));
			}
			else if(nextIs('='))
			{
				++index;
				tokens.push_back(MakeToken(TOKENTYPE_OPEQU, "=="));
			}
			else
			{
				tokens.push_back(MakeToken(TOKENTYPE_OPLET, "="));
			}
		}
		else if (current == '>')
		{
			if (nextIs('='))
			{
				++index;
				tokens.push_back(MakeToken(TOKENTYPE_OPGTE, ">="));
			}
			else
			{
				tokens.push_back(MakeToken(TOKENTYPE_OPGT, ">"));
			}
		}
		else if (current == '+')
		{
			tokens.push_back(MakeToken(TOKENTYPE_OPADD, "+"));
		}
		else if (current == '*')
		{
			tokens.push_back(MakeToken(TOKENTYPE_OPMUL, "*"));
		}
		else if (current == '/')
		{
			if (nextIs('/'))
			{
				++index;
				std::string comment = "//";
				while (hasNext() && !nextIs('\n'))
				{
					++index;
					comment += data[index];
				}
				if (preserveComments)
				{
					tokens.push_back(MakeToken(TOKENTYPE_COMMENT, comment));
				}
			}
			else if (nextIs('*'))
			{
				++index;
				std::string comment = "/*";
				bool ended = false;
				while (hasNext())
				{
					++index;
					comment += data[index];
					if (data[index] == '*' && nextIs('/'))
					{
						++index;
						comment += data[index];
						ended = true;
						break;
					}
				}
				if (!ended)
				{
					throw Error("EOF", "\"*/\"");
				}

				if (preserveComments)
				{
					tokens.push_back(MakeToken(TOKENTYPE_COMMENT, comment));
				}
			}
			else
			{
				tokens.push_back(MakeToken(TOKENTYPE_OPDIV, "/"));
			}
		}
		else if (current == '%')
		{
			tokens.push_back(MakeToken(TOKENTYPE_OPMOD, "%"));
		}
		else
		{
			throw Error(std::string() + "'" + current + "'", "A-Z, a-z, 0-9, '.', '$', '_', '(', ')', '!', '&', '|', '^', '~', '-', '<', '>', '=', '+', '*', '/', '%'");
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

SyntaxError Lexer::Error(std::string unexp, std::string exp)
{
	return SyntaxError("Lexer: Unexpected " + unexp + ", expecting " + exp + ".", lineNumber, index - lineStart);
}

Token Lexer::MakeToken(TokenType type, std::string word)
{
	Token token(type, word, lineNumber, tokenStart - lineStart, index - lineStart);
	return std::move(token);
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
	else if (str == "for")
	{
		return TOKENTYPE_FOR;
	}
	else if (str == "if")
	{
		return TOKENTYPE_IF;
	}
	else if (str == "while")
	{
		return TOKENTYPE_WHILE;
	}
	else if (str == "break")
	{
		return TOKENTYPE_BREAK;
	}
	else if (str == "continue")
	{
		return TOKENTYPE_CONTINUE;
	}
	else if (str == "else")
	{
		return TOKENTYPE_ELSE;
	}
	else if (str == "return")
	{
		return TOKENTYPE_RETURN;
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