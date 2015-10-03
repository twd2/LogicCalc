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
	Symbols.resize(symbols.size());
	std::sort(symbols.begin(), symbols.end());
	auto last = std::unique_copy(symbols.begin(), symbols.end(), Symbols.begin());
	Symbols.erase(last, Symbols.end());
	return std::move(code);
}

void GenerateVisitor::VisitNode(ASTNode* node)
{
	int size = node->Nodes.size();
	for (int i = 0; i < size; ++i)
	{
		VisitNode(node->Nodes[i]);
	}
	code.push_back(node->token);
	if (node->token.Type == TOKENTYPE_ID)
	{
		symbols.push_back(node->token.Value);
	}
}