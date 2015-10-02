#pragma once

#include <vector>
#include <string>
#include "StringHelper.h"
#include "SyntaxError.h"
#include "AST.h"
#include "ASTNode.h"
#include "Token.h"


class Parser
{
public:
	Parser(std::vector<Token> &tokens): tokens(tokens)
	{ 
		length = tokens.size();
	}

	~Parser();

	AST* parse();

	ASTNode* expr();
	ASTNode* tauimp_expr();
	ASTNode* dualimp_expr();
	ASTNode* imp_expr();
	ASTNode* or_expr();
	ASTNode* xor_expr();
	ASTNode* and_expr();
	ASTNode* bitor_expr();
	ASTNode* bitxor_expr();
	ASTNode* bitand_expr();
	ASTNode* not_expr();
	ASTNode* factor();

private:   
	std::vector<Token> &tokens;

	int length;
	int index = 0;

	void match(TokenType);

	Token& getToken();

	bool hasNext();
	std::string nextToken();
	bool nextIs(TokenType);

};

