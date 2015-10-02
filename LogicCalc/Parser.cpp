#include "stdafx.h"
#include "Parser.h"

Parser::~Parser()
{
}

AST Parser::parse()
{
	ASTNode* e = expr();

	if (e == NULL)
	{
		throw SyntaxError();
	}

	AST ast;
	ast.root = e;
	return ast;
}

ASTNode* Parser::expr()
{
	return tauimp_expr();
}

ASTNode* Parser::tauimp_expr()
{
	throw "not implemented";
}

ASTNode* Parser::imp_expr()
{
	throw "not implemented";
}

ASTNode* Parser::or_expr()
{
	throw "not implemented";
}

ASTNode* Parser::xor_expr()
{
	throw "not implemented";
}

ASTNode* Parser::and_expr()
{
	throw "not implemented";
}

ASTNode* Parser::bitor_expr()
{
	throw "not implemented";
}

ASTNode* Parser::bitxor_expr()
{
	if (index >= length)
	{
		throw SyntaxError("unexpected EOF");
	}
	Token &token = tokens[index];

	auto node1 = bitand_expr();
	while (true)
	{
		if (index >= length) //eps
		{
			break;
		}
		Token &token = tokens[index];

		if (token.Type == OPBITXOR)
		{
			Token &savedToken = token;
			match(OPBITXOR);
			auto node2 = bitand_expr();
			auto newNode = new ASTNode(savedToken);
			newNode->nodes.push_back(node1);
			newNode->nodes.push_back(node2);
			node1 = newNode;
			continue;
		}
		break; //eps
	}
	return node1;
}

ASTNode* Parser::bitand_expr()
{
	if (index >= length)
	{
	throw SyntaxError("unexpected EOF"); 
	}
	Token &token = tokens[index];

	auto node1 = not_expr();
	while (true)
	{
		if (index >= length) //eps
		{
			break;
		}
		Token &token = tokens[index];

		if (token.Type == OPBITAND)
		{
			Token &savedToken = token;
			match(OPBITAND);
			auto node2 = not_expr();
			auto newNode = new ASTNode(savedToken);
			newNode->nodes.push_back(node1);
			newNode->nodes.push_back(node2);
			node1 = newNode;
			continue;
		}
		break; //eps
	}
	return node1;
}

ASTNode* Parser::not_expr()
{
	ASTNode* node = NULL;
	if (index >= length)
	{
		throw SyntaxError("unexpected EOF, expecting ID, NUMBER, LBRACKET");
	}
	Token &token = tokens[index];

	switch (token.Type)
	{
	case OPNOT:
	{
		node = new ASTNode(token);
		match(OPNOT);
		auto subNode = not_expr();
		node->nodes.push_back(subNode);
		return node;
		break;
	}
	case OPBITNOT:
	{
		node = new ASTNode(token);
		match(OPBITNOT);
		auto subNode = not_expr();
		node->nodes.push_back(subNode);
		return node;
		break;
	}
	case ID:
	case NUMBER:
	case LBRACKET:
		return factor();
		break;
	default:
		throw SyntaxError("unexpected \"" + std::string(tokens[index].Value) + "\", expecting OPNOT, OPBITNOT, ID, NUMBER, LBRACKET");
		break;
	}
}

ASTNode* Parser::factor()
{
	ASTNode* node = NULL;
	if (index >= length)
	{
		throw SyntaxError("unexpected EOF, expecting ID, NUMBER, LBRACKET");
	}
	Token &token = tokens[index];

	switch (token.Type)
	{
	case ID:
		node = new ASTNode(token);
		match(ID);
		return node;
		break;
	case NUMBER:
		node = new ASTNode(token);
		match(NUMBER);
		return node;
		break;
	case LBRACKET:
		match(LBRACKET);
		node = expr();
		match(RBRACKET);
		return node;
		break;
	default:
		throw SyntaxError("unexpected \"" + std::string(tokens[index].Value) + "\", expecting ID, NUMBER, LBRACKET");
		break;
	}
}

void Parser::match(TokenType type)
{
	if (index < length && tokens[index].Type == type)
	{
		++index;
	}
	else
	{
		throw SyntaxError("unexpected " + ((index < length) ? ("\"" + std::string(tokens[index].Value) + "\"") : "EOF") + ", expecting " + toString(type));
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

std::string Parser::toString(int a)
{
	std::stringstream ss;
	ss << a;
	return ss.str();
}

bool Parser::nextIs(TokenType type)
{
	return hasNext() && tokens[index + 1].Type == type;
}