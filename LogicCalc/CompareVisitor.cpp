#include "stdafx.h"
#include "CompareVisitor.h"


CompareVisitor::CompareVisitor()
{
}


CompareVisitor::~CompareVisitor()
{
}

bool CompareVisitor::Compare(ASTNode *node1, ASTNode *node2)
{
	if (node1->token.Type != node2->token.Type) return false;
	if (node1->token.Value != node2->token.Value) return false;
	int size1 = node1->Nodes.size(),
		size2 = node2->Nodes.size();
	if (size1 != size2) return false;
	for (int i = 0; i < size1; ++i)
	{
		if (!Compare(node1->Nodes[i], node2->Nodes[i])) return false;
	}
	return true;
}