#pragma once

#include <sstream>
#include <string>
#include <vector>
#include "IdGenerator.h"

class GenerateVisitor
{
public:
	GenerateVisitor();
	~GenerateVisitor();

	std::vector<Token> Visit(AST*);
	IdGenerator Ids;

private:

	std::vector<std::string> symbols;
	std::vector<Token> code;

	void VisitNode(ASTNode*);
};

