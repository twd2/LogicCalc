// LogicCalc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

std::pair<std::vector<Token>, IdGenerator> compile(std::string data)
{
	Lexer lexer(data);
	std::vector<Token> &tokens = lexer.Do(false);

	Parser parser(tokens);
	AST *ast = parser.Parse();

	PruningVisitor::Visit(ast); //delete rubbish

	GenerateVisitor generator;
	std::vector<Token> &code = generator.Visit(ast); //generate

	delete ast;

	//we want to use lexerid as id.
	size_t size = code.size();
	for (size_t i = 0; i < size; ++i)
	{
		code[i].ID = code[i].LexerID;
	}

	return std::make_pair(code, lexer.Ids);
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

int calc(int *stack, std::vector<Token> &code, int values[])
{

#define STACK_PUSH(val) stack[++top]=(val)
#define STACK_TOP() (stack[top])
#define STACK_POP() (stack[top--])

	size_t size = code.size();

	int top = -1;

	int left, right;

	for (size_t i = 0; i < size; ++i)
	{
		Token &token = code[i];
		switch (token.Type)
		{
		case TOKENTYPE_ID:
			STACK_PUSH(values[token.ID]);
			break;
		case TOKENTYPE_INTNUMBER:
			STACK_PUSH(token.IntValue);
			break;
		case TOKENTYPE_FLOATNUMBER:
			throw RuntimeError("Float calculation is not implemented.");
			break;
		case TOKENTYPE_TRUE:
			STACK_PUSH(1);
			break;
		case TOKENTYPE_FALSE:
			STACK_PUSH(0);
			break;
		case TOKENTYPE_OPADD:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left + right);
			break;
		case TOKENTYPE_OPAND:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left && right);
			break;
		case TOKENTYPE_OPBITAND:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left & right);
			break;
		case TOKENTYPE_OPBITNOT:
			left = STACK_POP();
			STACK_PUSH(~left);
			break;
		case TOKENTYPE_OPBITOR:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left | right);
			break;
		case TOKENTYPE_OPBITXOR:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left ^ right);
			break;
		case TOKENTYPE_OPDIV:
			right = STACK_POP();
			left = STACK_POP();
			if (right == 0)
			{
				throw RuntimeError("Divided by zero.");
			}
			STACK_PUSH(left / right);
			break;
		case TOKENTYPE_OPDUALIMP:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH((left != 0) == (right != 0));
			break;
		case TOKENTYPE_OPEQU:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left == right);
			break;
		case TOKENTYPE_OPGT:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left > right);
			break;
		case TOKENTYPE_OPGTE:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left >= right);
			break;
		case TOKENTYPE_OPIMP:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(!left || right);
			break;
		case TOKENTYPE_OPLT:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left < right);
			break;
		case TOKENTYPE_OPLTE:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left <= right);
			break;
		case TOKENTYPE_OPMOD:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left % right);
			break;
		case TOKENTYPE_OPMUL:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left * right);
			break;
		case TOKENTYPE_OPNOT:
			left = STACK_POP();
			STACK_PUSH(!left);
			break;
		case TOKENTYPE_OPOR:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left || right);
			break;
		case TOKENTYPE_OPSUB:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH(left - right);
			break;
		case TOKENTYPE_OPTAUIMP:
			//prove needed
			break;
		case TOKENTYPE_OPXOR:
			right = STACK_POP();
			left = STACK_POP();
			STACK_PUSH((left != 0) != (right != 0));
			break;
		default:
			break;
		}
	}
	left = STACK_POP();
	return left;
}

Matrix *exprToTable(std::vector<Token> &code, IdGenerator &ids)
{
	std::vector<std::string> &symbols = ids.GetKeys();
	size_t symbolCount = symbols.size();

	int *values = new int[symbolCount];

	int count = 1 << symbolCount;

	Matrix *table = new Matrix(count, symbolCount + 1);
	Matrix &mat = *table;

	size_t codeSize = code.size();
	int *stack = new int[codeSize];

	for (int i = 0; i < count; ++i)
	{
		int *row = mat[i];
		//枚举可能的取值
		for (size_t id = 0; id < symbolCount; ++id)
		{
			values[id] = (i >> (symbolCount - id - 1)) & 1;
			row[id] = values[id];
		}

		int ans = calc(stack, code, values);

		row[symbolCount] = ans;
	}

	delete[] stack;
	delete[] values;
	return table;
}

void printTable(IdGenerator &ids, Matrix &table)
{
	std::vector<std::string> &symbols = ids.GetKeys();
	size_t symbolCount = symbols.size();

	for (size_t i = 0; i < symbolCount; ++i)
	{
		std::cout << symbols[i] << "\t";
	}
	std::cout << "expr" << std::endl;

	size_t count = table.m;

	for (size_t r = 0; r < count; ++r)
	{
		int *row = table[r];
		for (size_t i = 0; i < symbolCount; ++i)
		{
			std::cout << row[i] << "\t";
		}
		std::cout << row[symbolCount] << std::endl;
	}
}

std::string toDNF(IdGenerator &ids, Matrix &table)
{
	std::string result = "   ";

	std::vector<std::string> &symbols = ids.GetKeys();
	size_t symbolCount = symbols.size();
	size_t count = table.m;

	for (size_t r = 0; r < count; ++r)
	{
		int *row = table[r];

		if (row[symbolCount])
		{
			result += "(";
			for (size_t i = 0; i < symbolCount; ++i)
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

std::string toCNF(IdGenerator &ids, Matrix &table)
{
	std::string result = "   ";

	std::vector<std::string> &symbols = ids.GetKeys();
	size_t symbolCount = symbols.size();
	size_t count = table.m;

	for (ptrdiff_t r = count - 1; r >= 0; --r)
	{
		int *row = table[r];

		if (!row[symbolCount])
		{
			result += "(";
			for (size_t i = 0; i < symbolCount; ++i)
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
	do
	{
		std::string expr;
		std::cout << ">";
		std::getline(std::cin, expr);
		try
		{
			std::pair<std::vector<Token>, IdGenerator> codeSymbolsPair = compile(expr);
			std::vector<Token> &code = codeSymbolsPair.first;
			IdGenerator &ids = codeSymbolsPair.second;

			print(code);
			std::cout << std::endl;

			Matrix *table = exprToTable(code, ids);

			printTable(ids, *table);
			std::cout << std::endl;

			std::string DNF = toDNF(ids, *table);
			std::cout << "DNF:" << std::endl << DNF << std::endl << std::endl;

			std::string CNF = toCNF(ids, *table);
			std::cout << "CNF:" << std::endl << CNF << std::endl << std::endl;
			
			delete table;
		}
		catch (SyntaxError err)
		{
			if (err.what.length() > 0)
			{
				std::cout << "SyntaxError: " << err.what << std::endl;
			}
			else
			{
				std::cout << "SyntaxError" << std::endl;
			}
		}
		catch (RuntimeError err)
		{
			if (err.what.length() > 0)
			{
				std::cout << "RuntimeError: " << err.what << std::endl;
			}
			else
			{
				std::cout << "RuntimeError" << std::endl;
			}
		}
	} while (true);// (data != "");

	std::string s;
	std::getline(std::cin, s);

	return 0;
}
