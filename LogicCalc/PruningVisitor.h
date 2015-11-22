#pragma once

#include "AST.h"
#include "ASTNode.h"

class PruningVisitor
{
	
public:
	PruningVisitor();
	~PruningVisitor();

	static void Visit(AST*);
	static bool VisitNode(ASTNode**);

private:
	static bool VisitDualimpNode(ASTNode**);
	static bool VisitImpNode(ASTNode**);
	static bool VisitOrNode(ASTNode**);
	static bool VisitXorNode(ASTNode**);
	static bool VisitAndNode(ASTNode**);
	static bool VisitBitnotNode(ASTNode**);
	static bool VisitBitorNode(ASTNode**);
	static bool VisitBitxorNode(ASTNode**);
	static bool VisitBitandNode(ASTNode**);
	static bool VisitAddNode(ASTNode**);
	static bool VisitSubNode(ASTNode**);
	static bool VisitMulNode(ASTNode**);
	static bool VisitDivNode(ASTNode**);
	static bool VisitModNode(ASTNode**);
	static bool VisitNotNode(ASTNode**);
	static bool VisitIDNode(ASTNode**);
	static bool VisitIntNumberNode(ASTNode**);
	static bool VisitTrueNode(ASTNode**);
	static bool VisitFalseNode(ASTNode**);
	static void CopyTo(ASTNode*, ASTNode**);
	static void SetKnown(ASTNode*, int);

};

