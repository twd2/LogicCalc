// LogicCalc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


void test(std::string data)
{
	//std::string data = "(P||Q||1)&&!(P||Q||1)<->0"; //"(P->R)&&(Q->S)&&(P||Q)->R||S";

	std::cout << data << ":" << std::endl;

	//std::getline(std::cin, data);

	Lexer lex(data);

	auto &tokens = lex.Do();

	Parser parser(tokens);

	try
	{
		AST *result = parser.parse();
		
		PruningVisitor::Visit(result);
		
		PrintVisitor p;
		std::cout << p.Visit(result) << std::endl;

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
	/*test("0&&P");
	test("1&&P");
	test("P&&P");
	test("!P&&P");
	test("P&&!P");
	test("P&&(P||Q)");
	test("(P||Q)&&(P||Q)");
	test("(P||Q)&&P->P");

	test("0||P");
	test("1||P");
	test("P||P");
	test("!P||P");
	test("P||!P");
	test("P||(P&&Q)");
	test("(P&&Q)||P");
	test("(P&&Q)||P<->P");*/

	test("a^^1");
	test("(a^^1)&&a");
	test("1^^a");
	test("(0^^a)||(1^^a)");

	test("!(!P->!!!!!!!!!!P)->(!P->!!!!!!!!!!P)");

	test("~~~x");
	test("!!!!!!!!!!P");

	std::string s;
	std::getline(std::cin, s);

	return 0;
}

