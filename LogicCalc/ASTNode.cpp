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

bool ASTNode::operator==(const ASTNode &b)
{
	if (this->token != b.token) return false;

	size_t size1 = this->Nodes.size(),
		size2 = b.Nodes.size();
	if (size1 != size2) return false;

	for (size_t i = 0; i < size1; ++i)
	{
		if (*this->Nodes[i] != *b.Nodes[i]) return false;
	}
	return true;
}

bool ASTNode::operator!=(const ASTNode &b)
{
	return !(*this == b);
}