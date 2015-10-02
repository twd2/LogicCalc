#pragma once

#include <sstream>
#include <string>

class PrintVisitor
{
public:
	PrintVisitor();

	~PrintVisitor();

	std::string Visit(AST*);
	

private:
	std::stringstream ss;

	void VisitNode(ASTNode*);
};

