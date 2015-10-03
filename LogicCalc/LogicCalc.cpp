// LogicCalc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

std::pair<std::vector<Token>, std::vector<std::string> > compile(std::string data)
{
	std::vector<std::string> symbols;

	Lexer lexer(data);
	std::vector<Token> &tokens = lexer.Do();

	symbols = lexer.Symbols;

	Parser parser(tokens);
	AST *result = parser.Parse();

	PruningVisitor::Visit(result); //delete rubbish!

	GenerateVisitor generator;
	std::vector<Token> &code = generator.Visit(result); //generate!

	//symbols = generator.Symbols;

	delete result;

	return std::make_pair(code, symbols);
}

void print(std::vector<Token> &code)
{
	std::cout << "RPN: ";
	for each (const Token &token in code)
	{
		std::cout << token.Value << " ";
	}
	std::cout << std::endl;
}

int calc(std::vector<Token> &code, std::map<std::string, int> &values)
{
	std::stack<int> stack;

	int left, right;

	for each (const Token &token in code)
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

std::vector<std::vector<int> > exprToTable(std::vector<Token> &code, std::vector<std::string> &symbols)
{
	int symbolCount = symbols.size();

	std::map<std::string, int> values;

	int count = 1 << symbolCount;

	/*for (int id = 0; id < symbolCount; ++id)
	{
		std::cout << symbols[id] << " ";
	}
	std::cout << "expr" << std::endl;*/

	std::vector<std::vector<int> > table;
	table.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		std::vector<int> row;
		row.reserve(symbolCount + 1);
		//枚举可能的取值
		for (int id = 0; id < symbolCount; ++id)
		{
			row.push_back(values[symbols[id]] = (i >> (symbolCount - id - 1)) & 1);
		}

		row.push_back(calc(code, values));
		table.push_back(row);
		//std::cout << "   " << ans << std::endl;
	}
	return std::move(table);
}

void printTable(std::vector<std::string> &symbols, std::vector<std::vector<int> > &table)
{
	int symbolCount = symbols.size();

	for (int i = 0; i < symbolCount; ++i)
	{
		std::cout << symbols[i] << " ";
	}
	std::cout << "expr" << std::endl;

	for each (auto &row in table)
	{
		for (int i = 0; i < symbolCount; ++i)
		{
			std::cout << row[i] << " ";
		}
		std::cout << "   " << row[symbolCount] << std::endl;
	}
}

std::string toDNF(std::vector<std::string> &symbols, std::vector<std::vector<int> > &table)
{
	std::string result = "   ";

	int symbolCount = symbols.size();
	int count = table.size();

	for (int r = 0; r < count; ++r)
	{
		auto &row = table[r];

		if (row[symbolCount])
		{
			result += "(";
			for (int i = 0; i < symbolCount; ++i)
			{
				result += (row[i] ? "" : "!") + symbols[i];
				if (i < symbolCount - 1)
				{
					result += " && ";
				}
			}
			result += ")\n|| ";
		}
	}
	if (result == "   ")
	{
		result = "(empty)";
	}
	else
	{
		result = result.substr(0, result.length() - 4);
	}
	return std::move(result);
}

std::string toCNF(std::vector<std::string> &symbols, std::vector<std::vector<int> > &table)
{
	std::string result = "   ";

	int symbolCount = symbols.size();
	int count = table.size();

	for (int r = 0; r < count; ++r)
	{
		auto &row = table[r];

		if (!row[symbolCount])
		{
			result += "(";
			for (int i = 0; i < symbolCount; ++i)
			{
				result += (!row[i] ? "" : "!") + symbols[i];
				if (i < symbolCount - 1)
				{
					result += " || ";
				}
			}
			result += ")\n&& ";
		}
	}
	if (result == "   ")
	{
		result = "(empty)";
	}
	else
	{
		result = result.substr(0, result.length() - 4);
	}
	return std::move(result);
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string expr = "P||R&&Q||S->T";
	do
	{
		std::getline(std::cin, expr);
		try
		{
			std::pair<std::vector<Token>, std::vector<std::string> > codeSymbolsPair = compile(expr);
			std::vector<Token> &code = codeSymbolsPair.first;
			std::vector<std::string> &symbols = codeSymbolsPair.second;

			print(code);
			std::cout << std::endl;

			auto table = exprToTable(code, symbols);
			printTable(symbols, table);
			std::cout << std::endl;

			std::string DNF = toDNF(symbols, table);
			std::cout << "DNF:" << std::endl << DNF << std::endl << std::endl;

			std::string CNF = toCNF(symbols, table);
			std::cout << "CNF:" << std::endl << CNF << std::endl << std::endl;
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
	} while (true);// (data != "");

	std::string s;
	std::getline(std::cin, s);

	return 0;
}

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
