#pragma once
class CompareVisitor
{
public:
	CompareVisitor();
	~CompareVisitor();

	static bool Compare(ASTNode*, ASTNode*);
};

