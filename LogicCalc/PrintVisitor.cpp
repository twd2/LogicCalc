#include "stdafx.h"
#include "PrintVisitor.h"

PrintVisitor::PrintVisitor()
{
} 

PrintVisitor::~PrintVisitor()
{
}

std::string PrintVisitor::Visit(AST* ast)
{
	VisitNode(ast->root);
	return ss.str();
}

void PrintVisitor::VisitNode(ASTNode* node)
{
	int size = node->nodes.size();
	for (int i = 0; i < size; ++i)
	{
		VisitNode(node->nodes[i]);
	}
	ss << node->token.Value << std::endl;
}