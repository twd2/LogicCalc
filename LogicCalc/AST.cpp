#include "stdafx.h"
#include "AST.h"

AST::AST()
{
}

AST::~AST()
{
	if (root != NULL)
	{
		delete root;
		root = NULL;
	}
}
