#include "stdafx.h"
#include "ASTNode.h"

ASTNode::~ASTNode()
{
	int size = Nodes.size();
	for (int i = 0; i < size; ++i)
	{
		if (Nodes[i] != NULL)
		{
			delete Nodes[i];
			Nodes[i] = NULL;
		}
	}
}
