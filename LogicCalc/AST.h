#pragma once

#include "ASTNode.h"

class AST
{
public:

	ASTNode* root;

	AST();
	~AST();

private:

	void deleteNodes();
};

