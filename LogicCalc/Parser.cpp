#include "stdafx.h"
#include "Parser.h"

#define EXPR1(Operator, LowerExpr) \
auto node1 = LowerExpr();\
while (true)\
{\
	Token &token = getToken();\
\
	if (token.Type == Operator)\
	{\
		Token &savedToken = token;\
		match(Operator);\
		auto node2 = LowerExpr();\
		auto newNode = new ASTNode(savedToken);\
		newNode->nodes.push_back(node1);\
		newNode->nodes.push_back(node2);\
		node1 = newNode;\
		continue;\
	}\
	break; /*eps*/ \
}\
return node1;

Parser::~Parser()
{
}

AST* Parser::parse()
{
	ASTNode* e = expr();

	if (e == NULL)
	{
		throw SyntaxError();
	}

	AST *ast = new AST();
	ast->root = e;
	return ast;
}

ASTNode* Parser::expr()
{
	return tauimp_expr();
}

ASTNode* Parser::tauimp_expr()
{
	EXPR1(TOKENTYPE_OPTAUIMP, dualimp_expr);
}

ASTNode* Parser::dualimp_expr()
{
	EXPR1(TOKENTYPE_OPDUALIMP, imp_expr);
}

ASTNode* Parser::imp_expr()
{
	EXPR1(TOKENTYPE_OPIMP, or_expr);
}

ASTNode* Parser::or_expr()
{
	EXPR1(TOKENTYPE_OPOR, xor_expr);
}

ASTNode* Parser::xor_expr()
{
	EXPR1(TOKENTYPE_OPXOR, and_expr);
}

ASTNode* Parser::and_expr()
{
	EXPR1(TOKENTYPE_OPAND, bitor_expr);
}

ASTNode* Parser::bitor_expr()
{
	EXPR1(TOKENTYPE_OPBITOR, bitxor_expr);
}

ASTNode* Parser::bitxor_expr()
{
	EXPR1(TOKENTYPE_OPBITXOR, bitand_expr);
}

ASTNode* Parser::bitand_expr()
{
	EXPR1(TOKENTYPE_OPBITAND, not_expr);
}

ASTNode* Parser::not_expr()
{
	ASTNode* node = NULL;
	Token &token = getToken();

	switch (token.Type)
	{
	case TOKENTYPE_OPNOT:
	{
		node = new ASTNode(token);
		match(TOKENTYPE_OPNOT);
		auto subNode = not_expr();
		node->nodes.push_back(subNode);
		return node;
		break;
	}
	case TOKENTYPE_OPBITNOT:
	{
		node = new ASTNode(token);
		match(TOKENTYPE_OPBITNOT);
		auto subNode = not_expr();
		node->nodes.push_back(subNode);
		return node;
		break;
	}
	case TOKENTYPE_ID:
	case TOKENTYPE_NUMBER:
	case TOKENTYPE_LBRACKET:
		return factor();
		break;
	default:
		throw SyntaxError("unexpected \"" + std::string(token.Value) + "\", expecting OPNOT, OPBITNOT, ID, NUMBER, LBRACKET");
		break;
	}
}

ASTNode* Parser::factor()
{
	ASTNode* node = NULL;
	Token &token = getToken();

	switch (token.Type)
	{
	case TOKENTYPE_ID:
		node = new ASTNode(token);
		match(TOKENTYPE_ID);
		return node;
		break;
	case TOKENTYPE_NUMBER:
		node = new ASTNode(token);
		match(TOKENTYPE_NUMBER);
		return node;
		break;
	case TOKENTYPE_LBRACKET:
		match(TOKENTYPE_LBRACKET);
		node = expr();
		match(TOKENTYPE_RBRACKET);
		return node;
		break;
	default:
		throw SyntaxError("unexpected \"" + std::string(token.Value) + "\", expecting ID, NUMBER, LBRACKET");
		break;
	}
}

void Parser::match(TokenType type)
{
	Token &token = getToken();
	if (token.Type == type)
	{
		++index;
	}
	else
	{
		throw SyntaxError("unexpected \"" + std::string(token.Value) + "\", expecting " + StringHelper_toString(type));
	}
}

Token& Parser::getToken()
{
	if (index < length)
	{
		return tokens[index];
	}
	else
	{
		Token *token = new Token(TOKENTYPE_EOF, "EOF");
		return *token;
	}
}

bool Parser::hasNext()
{
	return index < length - 1;
}

std::string Parser::nextToken()
{
	if (!hasNext())
	{
		return "EOF";
	}
	else
	{
		return "'" + std::string(tokens[index + 1].Value) + "'";
	}
}

bool Parser::nextIs(TokenType type)
{
	return hasNext() && tokens[index + 1].Type == type;
}