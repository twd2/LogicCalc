// LogicCalc.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	std::string data = "~~!!~1^2&3&4&5&6&7";
	Lexer lex(data);

	auto tokens = lex.Do();

	Parser parser(tokens);

	auto result = parser.bitxor_expr();

	return 0;
}

