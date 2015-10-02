#include "stdafx.h"
#include "AST.h"

AST::AST()
{
}

AST::~AST()
{
	//std::cout << "deleting ast" << std::endl;
	if (root != NULL)
	{
		delete root;
		root = NULL;
	}
}
