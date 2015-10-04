#include "stdafx.h"
#include "GenerateVisitor.h"

GenerateVisitor::GenerateVisitor()
{
} 

GenerateVisitor::~GenerateVisitor()
{
}

std::vector<Token> GenerateVisitor::Visit(AST* ast)
{
	VisitNode(ast->root);
	return std::move(code);
}

void GenerateVisitor::VisitNode(ASTNode* node)
{
	size_t size = node->Nodes.size();
	for (size_t i = 0; i < size; ++i)
	{
		VisitNode(node->Nodes[i]);
	}
	if (node->token.Type == TOKENTYPE_ID)
	{
		node->token.ID = Ids.GetID(node->token.Value);
	}
	code.push_back(node->token);
}