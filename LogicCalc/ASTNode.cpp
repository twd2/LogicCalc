#include "stdafx.h"
#include "ASTNode.h"

ASTNode::~ASTNode()
{
	//std::cout << "deleting node" << std::endl;
	int size = nodes.size();
	for (int i = 0; i < size; ++i)
	{
		if (nodes[i] != NULL)
		{
			delete nodes[i];
			nodes[i] = NULL;
		}
	}
}
