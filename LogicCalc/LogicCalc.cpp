// LogicCalc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


void func()
{
	std::string data = "(P->R)&&(Q->S)&&(P||Q)->R||S";

	std::getline(std::cin, data);

	Lexer lex(data);

	auto &tokens = lex.Do();

	Parser parser(tokens);

	try
	{
		AST *result = parser.parse();
		std::cout << result->root->token.Value << std::endl;
		delete result;
	}
	catch (SyntaxError err)
	{
		if (err.why.length() > 0)
		{
			std::cout << "SyntaxError: " << err.why << std::endl;
		}
		else
		{
			std::cout << "SyntaxError" << std::endl;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	func();

	std::string s;
	std::getline(std::cin, s);

	return 0;
}

