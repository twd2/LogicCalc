#include "stdafx.h"
#include "Parser.h"

#define LEFT_ASSO_OP(TokenMatcher, LowerExpr) \
	ASTNode *node1 = LowerExpr();\
	while (true)\
	{\
		Token &token = getToken();\
	\
		TokenType type = token.Type;\
		if (TokenMatcher)\
		{\
			match(token.Type);\
			ASTNode *node2 = LowerExpr();\
			ASTNode *newNode = new ASTNode(token);\
			newNode->Nodes.push_back(node1);\
			newNode->Nodes.push_back(node2);\
			node1 = newNode;\
			continue;\
		}\
		else\
		{\
			/*eps*/\
		}\
		break; \
	}\
	return node1;

Parser::~Parser()
{
}

AST* Parser::Parse()
{
	ASTNode *rootNode = expr();

	if (rootNode == NULL)
	{
		throw SyntaxError();
	}

	AST *tree = new AST();
	tree->root = rootNode;
	return tree;
}

ASTNode* Parser::expr()
{
	return tauimp_expr();
}

ASTNode* Parser::tauimp_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPTAUIMP, dualimp_expr);
}

ASTNode* Parser::dualimp_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPDUALIMP, imp_expr);
}

ASTNode* Parser::imp_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPIMP, or_expr);
}

ASTNode* Parser::or_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPOR, xor_expr);
}

ASTNode* Parser::xor_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPXOR, and_expr);
}

ASTNode* Parser::and_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPAND, bitor_expr);
}

ASTNode* Parser::bitor_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPBITOR, bitxor_expr);
}

ASTNode* Parser::bitxor_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPBITXOR, bitand_expr);
}

ASTNode* Parser::bitand_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPBITAND, addsub_expr);
}

ASTNode* Parser::addsub_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPADD || type == TOKENTYPE_OPSUB, muldivmod_expr);
}

ASTNode* Parser::muldivmod_expr()
{
	LEFT_ASSO_OP(type == TOKENTYPE_OPMUL || type == TOKENTYPE_OPDIV || type == TOKENTYPE_OPMOD, unary_expr);
}

ASTNode* Parser::unary_expr()
{
	ASTNode* node = NULL;
	Token &token = getToken();

	switch (token.Type)
	{
	case TOKENTYPE_OPNOT:
	case TOKENTYPE_OPBITNOT:
	{
		node = new ASTNode(token);
		match(token.Type);
		ASTNode *subNode = unary_expr();
		node->Nodes.push_back(subNode);
		return node;
		break;
	}
	case TOKENTYPE_ID:
	case TOKENTYPE_NUMBER:
	case TOKENTYPE_LBRACKET:
		return factor();
		break;
	default:
		throw SyntaxError("Parser: Unexpected " + tokenString() + ", expecting OPNOT, OPBITNOT, ID, NUMBER, LBRACKET");
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
		throw SyntaxError("Parser: Unexpected " + tokenString() + ", expecting ID, NUMBER, LBRACKET");
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
		throw SyntaxError("Parser: Unexpected " + tokenString() + ", expecting " + Token_toName(type));
	}
}

Token &Parser::getToken()
{
	if (index < length)
	{
		return tokens[index];
	}
	else
	{
		return EOFToken;
	}
}

bool Parser::hasNext()
{
	return index < length - 1;
}

std::string Parser::tokenString()
{
	Token &token = getToken();
	if (token.Type == TOKENTYPE_EOF)
	{
		return "EOF";
	}
	else
	{
		return "\"" + token.Value + "\"";
	}
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