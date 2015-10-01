// LogicCalc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	std::string data = "A &%^&%^%^ ^^^%^& B 123CD233(P->Q)&&(R->Q)&&P=>Q654";
	Lexer lex(data);

	auto result = lex.Do();

	return 0;
}

