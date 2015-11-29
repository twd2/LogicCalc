#include "stdafx.h"
#include "Parser.h"

#define LEFT_ASSO_OP(ExprType, TokenMatcher, LowerExpr) \
	ASTNode *node1 = LowerExpr();\
/*_Rest*/ \
	while (true)\
	{\
		const Token &token = getToken();\
	\
		TokenType type = token.Type;\
		if (TokenMatcher)\
		{\
			match(token.Type);\
			ASTNode *node2 = LowerExpr();\
			ExprType *newNode = new ExprType(token);\
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

AST *Parser::Parse()
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

ASTNode *Parser::expr()
{
	return dualimp_expr();
}

ASTNode *Parser::tauimp_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPTAUIMP, dualimp_expr);
}

ASTNode *Parser::dualimp_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPDUALIMP, imp_expr);
}

ASTNode *Parser::imp_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPIMP, or_expr);
}

ASTNode *Parser::or_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPOR, xor_expr);
}

ASTNode *Parser::xor_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPXOR, and_expr);
}

ASTNode *Parser::and_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPAND, bitor_expr);
}

ASTNode *Parser::bitor_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPBITOR, bitxor_expr);
}

ASTNode *Parser::bitxor_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPBITXOR, bitand_expr);
}

ASTNode *Parser::bitand_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPBITAND, addsub_expr);
}

ASTNode *Parser::addsub_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPADD || type == TOKENTYPE_OPSUB, muldivmod_expr);
}

ASTNode *Parser::muldivmod_expr()
{
	LEFT_ASSO_OP(ASTNode, type == TOKENTYPE_OPMUL || type == TOKENTYPE_OPDIV || type == TOKENTYPE_OPMOD, unary_expr);
}

ASTNode *Parser::unary_expr()
{
	ASTNode *node = NULL;
	const Token &token = getToken();

	switch (token.Type)
	{
	case TOKENTYPE_OPNOT:
	case TOKENTYPE_OPBITNOT:
	case TOKENTYPE_OPADD:
	case TOKENTYPE_OPSUB:
	{
		node = new ASTNode(token);
		match(token.Type);
		ASTNode *subNode = unary_expr();
		node->Nodes.push_back(subNode);
		return node;
		break;
	}
	// TODO: getaddress & dereference
	case TOKENTYPE_ID:
	case TOKENTYPE_INTNUMBER:
	case TOKENTYPE_FLOATNUMBER:
	case TOKENTYPE_TRUE:
	case TOKENTYPE_FALSE:
	case TOKENTYPE_LROUNDBRACKET:
		return factor();
		break;
	default:
		throw Error("OPNOT, OPBITNOT, OPADD, OPSUB, ID, INTNUMBER, FLOATNUMBER, TRUE, FALSE, LBRACKET");
		break;
	}
}

ASTNode *Parser::factor()
{
	ASTNode *node = NULL;
	const Token &token = getToken();

	switch (token.Type)
	{
	case TOKENTYPE_ID:
	case TOKENTYPE_INTNUMBER:
	case TOKENTYPE_FLOATNUMBER:
	case TOKENTYPE_TRUE:
	case TOKENTYPE_FALSE:
		node = new ASTNode(token);
		match(token.Type);
		return node;
		break;
	case TOKENTYPE_LROUNDBRACKET:
		match(TOKENTYPE_LROUNDBRACKET);
		node = expr();
		match(TOKENTYPE_RROUNDBRACKET);
		return node;
		break;
	default:
		throw Error("ID, INTNUMBER, FLOATNUMBER, TRUE, FALSE, LBRACKET");
		break;
	}
}

void Parser::match(TokenType type)
{
	const Token &token = getToken();
	if (token.Type == type)
	{
		++index;
	}
	else
	{
		throw Error(Token::Name(type));
	}
}

const Token &Parser::getToken()
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
	return tokenString(getToken());
}

std::string Parser::tokenString(const Token &token)
{
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

SyntaxError Parser::Error(std::string exp)
{
	return Error(getToken(), exp);
}

SyntaxError Parser::Error(const Token &token, std::string exp)
{
	return SyntaxError("Parser: Unexpected " + tokenString(token) + ", expecting " + exp, token.LineNumber, token.ColumnStart, token.ColumnEnd);
}
