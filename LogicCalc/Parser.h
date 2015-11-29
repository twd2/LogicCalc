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
	Parser(const std::vector<Token> &tokens): tokens(tokens)
	{ 
		length = tokens.size();
	}

	~Parser();

	AST* Parse();

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
	ASTNode* addsub_expr();
	ASTNode* muldivmod_expr();
	ASTNode* unary_expr();
	ASTNode* factor();

private:   

	Token EOFToken = Token(TOKENTYPE_EOF, "EOF");

	const std::vector<Token> &tokens;

	size_t length;
	size_t index = 0;

	void match(TokenType);

	const Token &getToken();

	bool hasNext();
	std::string tokenString();
	std::string tokenString(const Token&);
	std::string nextToken();
	bool nextIs(TokenType);

	SyntaxError Error(std::string);
	SyntaxError Error(const Token&, std::string);

};

