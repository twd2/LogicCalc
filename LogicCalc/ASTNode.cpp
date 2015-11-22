#include "stdafx.h"
#include "ASTNode.h"

ASTNode::~ASTNode()
{
	size_t size = Nodes.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (Nodes[i] != NULL)
		{
			delete Nodes[i];
			Nodes[i] = NULL;
		}
	}
}

