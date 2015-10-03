#pragma once

#include <sstream>
#include <string>

class GenerateVisitor
{
public:
	GenerateVisitor();
	~GenerateVisitor();

	std::vector<Token> Visit(AST*);
	std::vector<std::string> Symbols;

private:

	std::vector<std::string> symbols;
	std::vector<Token> code;

	void VisitNode(ASTNode*);
};

