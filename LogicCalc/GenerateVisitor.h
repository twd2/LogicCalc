#pragma once

#include <sstream>
#include <string>

class GenerateVisitor
{
public:
	GenerateVisitor();
	~GenerateVisitor();

	std::vector<Token> Visit(AST*);
	

private:
	std::vector<Token> code;

	void VisitNode(ASTNode*);
};

