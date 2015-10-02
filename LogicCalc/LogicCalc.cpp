// LogicCalc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


std::vector<Token> compile(std::string data)
{
	Lexer lex(data);

	auto &tokens = lex.Do();

	Parser parser(tokens);

	AST *result = parser.parse();

	PruningVisitor::Visit(result); //delete rubbish!

	GenerateVisitor generator;
	auto &code = generator.Visit(result); //generate!

	delete result;

	return std::move(code);
}

void print(std::vector<Token> &code)
{
	for each (auto token in code)
	{
		std::cout << token.Value << " ";
	}
	std::cout << std::endl;
}

int calc(std::vector<Token> &code, std::map<std::string, int> &values)
{
	std::stack<int> stack;

	int left, right;

	for each (auto token in code)
	{
		switch (token.Type)
		{
		case TOKENTYPE_ID:
			stack.push(values[token.Value]);
			break;
		case TOKENTYPE_NUMBER:
			stack.push(StringHelper_toInt(token.Value));
			break;
		case TOKENTYPE_OPADD:
			break;
		case TOKENTYPE_OPAND:
			left = stack.top();
			stack.pop();
			right = stack.top();
			stack.pop();
			stack.push(left && right);
			break;
		case TOKENTYPE_OPBITAND:
			left = stack.top();
			stack.pop();
			right = stack.top();
			stack.pop();
			stack.push(left & right);
			break;
		case TOKENTYPE_OPBITNOT:
			left = stack.top();
			stack.pop();
			stack.push(~left);
			break;
		case TOKENTYPE_OPBITOR:
			left = stack.top();
			stack.pop();
			right = stack.top();
			stack.pop();
			stack.push(left | right);
			break;
		case TOKENTYPE_OPBITXOR:
			left = stack.top();
			stack.pop();
			right = stack.top();
			stack.pop();
			stack.push(left ^ right);
			break;
		case TOKENTYPE_OPDIV:
			break;
		case TOKENTYPE_OPDUALIMP:
			left = stack.top();
			stack.pop();
			right = stack.top();
			stack.pop();
			stack.push((left != 0) == (right != 0));
			break;
		case TOKENTYPE_OPEQU:
			break;
		case TOKENTYPE_OPGT:
			break;
		case TOKENTYPE_OPGTE:
			break;
		case TOKENTYPE_OPIMP:
			left = stack.top();
			stack.pop();
			right = stack.top();
			stack.pop();
			stack.push(!left || right);
			break;
		case TOKENTYPE_OPLT:
			break;
		case TOKENTYPE_OPLTE:
			break;
		case TOKENTYPE_OPMOD:
			break;
		case TOKENTYPE_OPMUL:
			break;
		case TOKENTYPE_OPNOT:
			left = stack.top();
			stack.pop();
			stack.push(!left);
			break;
		case TOKENTYPE_OPOR:
			left = stack.top();
			stack.pop();
			right = stack.top();
			stack.pop();
			stack.push(left || right);
			break;
		case TOKENTYPE_OPSUB:
			break;
		case TOKENTYPE_OPTAUIMP:
			break;
		case TOKENTYPE_OPXOR:
			left = stack.top();
			stack.pop();
			right = stack.top();
			stack.pop();
			stack.push((left != 0) != (right != 0));
			break;
		default:
			break;
		}
	}
	left = stack.top();
	stack.pop();
	return left;
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		std::vector<Token> code;
		/*code = compile("0&&P");
		print(code);
		code = compile("1&&P");
		print(code);
		code = compile("P&&P");
		print(code);
		code = compile("!P&&P");
		print(code);
		code = compile("P&&!P");
		print(code);
		code = compile("P&&(P||Q)");
		print(code);
		code = compile("(P||Q)&&(P||Q)");
		print(code);
		code = compile("(P||Q)&&P->P");
		print(code);
		code = compile("0||P");
		print(code);
		code = compile("1||P");
		print(code);
		code = compile("P||P");
		print(code);
		code = compile("!P||P");
		print(code);
		code = compile("P||!P");
		print(code);
		code = compile("P||(P&&Q)");
		print(code);
		code = compile("(P&&Q)||P");
		print(code);
		code = compile("(P&&Q)||P<->P");
		print(code);
		code = compile("a^^1");
		print(code);
		code = compile("(a^^1)&&a");
		print(code);
		code = compile("1^^a");
		print(code);
		code = compile("(0^^a)||(1^^a)");
		print(code);
		code = compile("!(!P->!!!!!!!!!!P)->(!P->!!!!!!!!!!P)");
		print(code);
		code = compile("~~~x");
		print(code);
		code = compile("!!!!!!!!!!P");
		print(code);
		code = compile("(P||Q||1)&&!(P||Q||1)<->0");
		print(code);*/
		for (int i = 0; i < 10000; ++i)
		{
			code = compile("(P->R)&&(Q->S)&&(P||Q)->R||S");
			//print(code);
			/*std::map<std::string, int> values;
			values["P"] = 0;
			values["Q"] = 0;
			values["R"] = 0;
			values["S"] = 0;
			int ans = calc(code, values);*/
			//std::cout << ans << std::endl;
		}
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

	std::string s;
	//std::getline(std::cin, s);

	return 0;
}

