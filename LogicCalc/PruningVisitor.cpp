#include "stdafx.h"
#include "PruningVisitor.h"


PruningVisitor::PruningVisitor()
{
}


PruningVisitor::~PruningVisitor()
{
}

void PruningVisitor::Visit(AST *ast)
{
	VisitNode(&(ast->root));
}

/*
	VisitNode 会修改 node, 因此需要调用完毕之后再 *node
*/
bool PruningVisitor::VisitNode(ASTNode **nodePtr)
{
	switch ((*nodePtr)->token.Type)
	{
	case TOKENTYPE_ID:
		return VisitIDNode(nodePtr);
		break;
	case TOKENTYPE_INTNUMBER:
		return VisitIntNumberNode(nodePtr);
		break;
	case TOKENTYPE_FLOATNUMBER:
		// TODO: float number
		break;
	case TOKENTYPE_TRUE:
		return VisitTrueNode(nodePtr);
		break;
	case TOKENTYPE_FALSE:
		return VisitFalseNode(nodePtr);
		break;
	case TOKENTYPE_OPADD:
		return VisitAddNode(nodePtr);
		break;
	case TOKENTYPE_OPAND:
		return VisitAndNode(nodePtr);
		break;
	case TOKENTYPE_OPBITAND:
		return VisitBitandNode(nodePtr);
		break;
	case TOKENTYPE_OPBITNOT:
		return VisitBitnotNode(nodePtr);
		break;
	case TOKENTYPE_OPBITOR:
		return VisitBitorNode(nodePtr);
		break;
	case TOKENTYPE_OPBITXOR:
		return VisitBitxorNode(nodePtr);
		break;
	case TOKENTYPE_OPDIV:
		return VisitDivNode(nodePtr);
		break;
	case TOKENTYPE_OPDUALIMP:
		return VisitDualimpNode(nodePtr);
		break;
	case TOKENTYPE_OPEQU:
		throw "Not Implemented";
		break;
	case TOKENTYPE_OPGT:
		throw "Not Implemented";
		break;
	case TOKENTYPE_OPGTE:
		throw "Not Implemented";
		break;
	case TOKENTYPE_OPIMP:
		return VisitImpNode(nodePtr);
		break;
	case TOKENTYPE_OPLT:
		throw "Not Implemented";
		break;
	case TOKENTYPE_OPLTE:
		throw "Not Implemented";
		break;
	case TOKENTYPE_OPMOD:
		return VisitModNode(nodePtr);
		break;
	case TOKENTYPE_OPMUL:
		return VisitMulNode(nodePtr);
		break;
	case TOKENTYPE_OPNOT:
		return VisitNotNode(nodePtr);
		break;
	case TOKENTYPE_OPOR:
		return VisitOrNode(nodePtr);
		break;
	case TOKENTYPE_OPSUB:
		return VisitSubNode(nodePtr);
		break;
	case TOKENTYPE_OPTAUIMP:
		break;
	case TOKENTYPE_OPXOR:
		return VisitXorNode(nodePtr);
		break;
	default:
		break;
	}
	return false;
}

bool PruningVisitor::VisitDualimpNode(ASTNode **nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);

	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if (leftNodeKnown && rightNodeKnown)
	{
		bool value = leftNode->Value == rightNode->Value;

		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		node->Nodes.clear();
		SetKnown(node, value);
		return true;
	}
	else if (CompareVisitor::Compare(leftNode, rightNode)) //P<->P === 1
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		node->Nodes.clear();
		SetKnown(node, 1);
		return true;
	}
	else if ((leftNodeKnown && leftNode->Value == 0)) //0<->x === !x
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPNOT, "!"));
		newNode->Nodes.push_back(rightNode);

		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodePtr);

		return newKnown;
	}
	else if (leftNodeKnown && leftNode->Value == 1) //1<->x === x
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;

		CopyTo(rightNode, nodePtr);

		return rightNodeKnown;
	}
	else if (rightNodeKnown && rightNode->Value == 0) //x<->0 === !x
	{
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPNOT, "!"));
		newNode->Nodes.push_back(leftNode);

		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodePtr);

		return newKnown;
	}
	else if (rightNodeKnown && rightNode->Value == 1) //x<->1 === x
	{
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		CopyTo(leftNode, nodePtr);

		return leftNodeKnown;
	}

	return false;
}

bool PruningVisitor::VisitImpNode(ASTNode **nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);

	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	//P->Q === !P||Q
	ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPOR, "||"));

	ASTNode *newSubNode = new ASTNode(Token(TOKENTYPE_OPNOT, "!"));
	newSubNode->Nodes.push_back(leftNode);

	newNode->Nodes.push_back(newSubNode);
	newNode->Nodes.push_back(rightNode);

	bool newKnown = VisitNode(&newNode);

	CopyTo(newNode, nodePtr);

	return newKnown;
}

bool PruningVisitor::VisitOrNode(ASTNode **nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);

	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if ((leftNodeKnown && leftNode->Value == 1) || (rightNodeKnown && rightNode->Value == 1))
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;
		node->Nodes.clear();
		SetKnown(node, 1);
		return true;
	}
	else if (leftNodeKnown && leftNode->Value == 0)
	{
		if (rightNodeKnown && rightNode->Value == 0)
		{
			delete leftNode;
			*leftNodePtr = leftNode = NULL;
			delete rightNode;
			*rightNodePtr = rightNode = NULL;
			node->Nodes.clear();
			SetKnown(node, 0);
			return true;
		}
		else //!rightNodeKnown
		{
			delete leftNode;
			*leftNodePtr = leftNode = NULL;
			CopyTo(rightNode, nodePtr);
			return false;
		}
	}
	else if (rightNodeKnown && rightNode->Value == 0) //!leftNodeHasValue
	{
		delete rightNode;
		*rightNodePtr = rightNode = NULL;
		CopyTo(leftNode, nodePtr);
		return false;
	}
	else //!leftNodeKnown && !rightNodeKnown
	{
		if (CompareVisitor::Compare(leftNode, rightNode)) //P||P===P
		{
			delete rightNode;
			*rightNodePtr = rightNode = NULL;
			CopyTo(leftNode, nodePtr);
			return false;
		}
		else if (leftNode->token.Type == TOKENTYPE_OPNOT &&
			CompareVisitor::Compare(leftNode->Nodes[0], rightNode)) //!P||P===1
		{
			delete leftNode;
			*leftNodePtr = leftNode = NULL;
			delete rightNode;
			*rightNodePtr = rightNode = NULL;
			node->Nodes.clear();
			SetKnown(node, 1);
			return true;
		}
		else if (rightNode->token.Type == TOKENTYPE_OPNOT &&
			CompareVisitor::Compare(rightNode->Nodes[0], leftNode)) //P||!P===1
		{
			delete leftNode;
			*leftNodePtr = leftNode = NULL;
			delete rightNode;
			*rightNodePtr = rightNode = NULL;
			node->Nodes.clear();
			SetKnown(node, 1);
			return true;
		}
		else if (leftNode->token.Type == TOKENTYPE_OPAND &&
			(CompareVisitor::Compare(leftNode->Nodes[0], rightNode) ||
			CompareVisitor::Compare(leftNode->Nodes[1], rightNode))) //(P&&Q)||P===P, (Q&&P)||P===P
		{
			delete leftNode;
			*leftNodePtr = leftNode = NULL;
			CopyTo(rightNode, nodePtr);
			return false;
		}
		else if (rightNode->token.Type == TOKENTYPE_OPAND &&
			(CompareVisitor::Compare(rightNode->Nodes[0], leftNode) ||
			CompareVisitor::Compare(rightNode->Nodes[1], leftNode))) //P||(P&&Q)===P, P||(Q&&P)===P
		{
			delete rightNode;
			*rightNodePtr = rightNode = NULL;
			CopyTo(leftNode, nodePtr);
			return false;
		}
	}
	return false;
}

bool PruningVisitor::VisitXorNode(ASTNode **nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);

	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if (leftNodeKnown && rightNodeKnown)
	{
		bool value = leftNode->Value != rightNode->Value;
		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;
		node->Nodes.clear();
		SetKnown(node, value);
		return true;
	}
	else if (CompareVisitor::Compare(leftNode, rightNode)) //P^^P === 0
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;
		node->Nodes.clear();
		SetKnown(node, 0);
		return true;
	}
	else if ((leftNodeKnown && leftNode->Value == 1)) //1^^x === !x
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPNOT, "!"));
		newNode->Nodes.push_back(rightNode);

		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodePtr);

		return newKnown;
	}
	else if (leftNodeKnown && leftNode->Value == 0) //0^x === x
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;

		CopyTo(rightNode, nodePtr);

		return rightNodeKnown;
	}
	else if (rightNodeKnown && rightNode->Value == 1) //x^1 === !x
	{
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPNOT, "!"));
		newNode->Nodes.push_back(leftNode);

		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodePtr);

		return newKnown;
	}
	else if (rightNodeKnown && rightNode->Value == 0) //x^0 === x
	{
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		CopyTo(leftNode, nodePtr);

		return leftNodeKnown;
	}
	return false;
}

bool PruningVisitor::VisitAndNode(ASTNode **nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);

	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if ((leftNodeKnown && leftNode->Value == 0) || (rightNodeKnown && rightNode->Value == 0)) // 0&&P === 0, P&&0 === 0
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;
		node->Nodes.clear();
		SetKnown(node, 0);
		return true;
	}
	else if (leftNodeKnown && leftNode->Value == 1)
	{
		if (rightNodeKnown && rightNode->Value == 1) //1&&1 === 1
		{
			delete leftNode;
			*leftNodePtr = leftNode = NULL;
			delete rightNode;
			*rightNodePtr = rightNode = NULL;
			node->Nodes.clear();
			SetKnown(node, 1);
			return true;
		}
		else  //1&&P === P
		{
			delete leftNode;
			*leftNodePtr = leftNode = NULL;
			CopyTo(rightNode, nodePtr);
			return false;
		}
	}
	else if (rightNodeKnown && rightNode->Value == 1) //P&&1 === P
	{
		delete rightNode;
		*rightNodePtr = rightNode = NULL;
		CopyTo(leftNode, nodePtr);
		return false;
	}
	else
	{
		if (CompareVisitor::Compare(leftNode, rightNode)) //P&&P===P
		{
			delete rightNode;
			*rightNodePtr = rightNode = NULL;
			CopyTo(leftNode, nodePtr);
			return false;
		}
		else if (leftNode->token.Type == TOKENTYPE_OPNOT &&
			CompareVisitor::Compare(leftNode->Nodes[0], rightNode)) //!P&&P===0
		{
			delete leftNode;
			*leftNodePtr = leftNode = NULL;
			delete rightNode;
			*rightNodePtr = rightNode = NULL;
			node->Nodes.clear();
			SetKnown(node, 0);
			return true;
		}
		else if (rightNode->token.Type == TOKENTYPE_OPNOT &&
			CompareVisitor::Compare(rightNode->Nodes[0], leftNode)) //P&&!P===0
		{
			delete leftNode;
			*leftNodePtr = leftNode = NULL;
			delete rightNode;
			*rightNodePtr = rightNode = NULL;
			node->Nodes.clear();
			SetKnown(node, 0);
			return true;
		}
		else if (leftNode->token.Type == TOKENTYPE_OPOR &&
			(CompareVisitor::Compare(leftNode->Nodes[0], rightNode) ||
			CompareVisitor::Compare(leftNode->Nodes[1], rightNode))) //(P||Q)&&P===P, (Q||P)&&P===P
		{
			delete leftNode;
			*leftNodePtr = leftNode = NULL;
			CopyTo(rightNode, nodePtr);
			return false;
		}
		else if (rightNode->token.Type == TOKENTYPE_OPOR &&
			(CompareVisitor::Compare(rightNode->Nodes[0], leftNode) ||
			CompareVisitor::Compare(rightNode->Nodes[1], leftNode))) //P&&(P||Q)===P, P&&(Q||P)===P
		{
			delete rightNode;
			*rightNodePtr = rightNode = NULL;
			CopyTo(leftNode, nodePtr);
			return false;
		}
	}
	return false;
}

bool PruningVisitor::VisitBitnotNode(ASTNode **nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **subnodePtr = &(node->Nodes[0]);
	bool subKnown = VisitNode(subnodePtr);
	ASTNode *subNode = *subnodePtr;
	if (subKnown)
	{
		int value = ~subNode->Value;
		delete subNode;
		node->Nodes.clear();
		SetKnown(node, value);
		return true;
	}
	else if (subNode->token.Type == TOKENTYPE_OPBITNOT) //~~x === x
	{
		ASTNode *subSubNode = subNode->Nodes[0];
		subNode->Nodes.clear();
		delete subNode;
		*subnodePtr = subNode = NULL;

		CopyTo(subSubNode, nodePtr);
		return subSubNode->Known;
	}
	return false;
}

bool PruningVisitor::VisitBitorNode(ASTNode **nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);
	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if ((leftNodeKnown && leftNode->Value == 0xFFFFFFFF) || (rightNodeKnown && rightNode->Value == 0xFFFFFFFF))
	{
		delete leftNode;
		delete rightNode;
		node->Nodes.clear();
		SetKnown(node, 0xFFFFFFFF);
		return true;
	}
	else if (leftNodeKnown && leftNode->Value == 0)
	{
		delete leftNode;
		CopyTo(rightNode, nodePtr);
		return false;
	}
	else if (rightNodeKnown && rightNode->Value == 0)
	{
		delete rightNode;
		CopyTo(leftNode, nodePtr);
		return false;
	}
	return false;
}

bool PruningVisitor::VisitBitxorNode(ASTNode **nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);
	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if ((leftNodeKnown && leftNode->Value == 0xFFFFFFFF)) //1^x === ~x
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPBITNOT, "~"));
		newNode->Nodes.push_back(rightNode);
		
		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodePtr);

		return newKnown;
	}
	else if (leftNodeKnown && leftNode->Value == 0) //0^x === x
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;

		CopyTo(rightNode, nodePtr);

		return rightNodeKnown;
	}
	else if (rightNodeKnown && rightNode->Value == 0xFFFFFFFF) //x^1 === ~x
	{
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPBITNOT, "~"));
		newNode->Nodes.push_back(leftNode);

		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodePtr);

		return newKnown;
	}
	else if (rightNodeKnown && rightNode->Value == 0) //x^0 === x
	{
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		CopyTo(leftNode, nodePtr);

		return leftNodeKnown;
	}
	return false;
}

bool PruningVisitor::VisitBitandNode(ASTNode **nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);
	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if ((leftNodeKnown && leftNode->Value == 0) || (rightNodeKnown && rightNode->Value == 0))
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		node->Nodes.clear();
		SetKnown(node, 0);
		return true;
	}
	else if (leftNodeKnown && leftNode->Value == 0xFFFFFFFF)
	{
		delete leftNode;
		*leftNodePtr = leftNode = NULL;

		CopyTo(rightNode, nodePtr);
		return rightNodeKnown;
	}
	else if (rightNodeKnown && rightNode->Value == 0xFFFFFFFF) 
	{
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		CopyTo(leftNode, nodePtr);
		return leftNodeKnown;
	}
	return false;
}

bool PruningVisitor::VisitAddNode(ASTNode**nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);
	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if (leftNodeKnown && rightNodeKnown)
	{
		int value = leftNode->Value + rightNode->Value;

		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		node->Nodes.clear();
		SetKnown(node, value);
		return true;
	}
	return false;
}

bool PruningVisitor::VisitSubNode(ASTNode**nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);
	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if (leftNodeKnown && rightNodeKnown)
	{
		int value = leftNode->Value - rightNode->Value;

		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		node->Nodes.clear();
		SetKnown(node, value);
		return true;
	}
	return false;
}

bool PruningVisitor::VisitMulNode(ASTNode**nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);
	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if (leftNodeKnown && rightNodeKnown)
	{
		int value = leftNode->Value * rightNode->Value;

		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		node->Nodes.clear();
		SetKnown(node, value);
		return true;
	}
	return false;
}

bool PruningVisitor::VisitDivNode(ASTNode**nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);
	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if (leftNodeKnown && rightNodeKnown)
	{
		if (rightNode->Value == 0)
		{
			throw RuntimeError("Divided by zero.");
		}
		int value = leftNode->Value / rightNode->Value;

		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		node->Nodes.clear();
		SetKnown(node, value);
		return true;
	}
	return false;
}

bool PruningVisitor::VisitModNode(ASTNode**nodePtr)
{
	ASTNode *node = *nodePtr;

	ASTNode **leftNodePtr, **rightNodePtr;
	leftNodePtr = &(node->Nodes[0]);
	rightNodePtr = &(node->Nodes[1]);
	bool leftNodeKnown = VisitNode(leftNodePtr),
		rightNodeKnown = VisitNode(rightNodePtr);

	ASTNode *leftNode = *leftNodePtr,
		*rightNode = *rightNodePtr;

	if (leftNodeKnown && rightNodeKnown)
	{
		int value = leftNode->Value % rightNode->Value;

		delete leftNode;
		*leftNodePtr = leftNode = NULL;
		delete rightNode;
		*rightNodePtr = rightNode = NULL;

		node->Nodes.clear();
		SetKnown(node, value);
		return true;
	}
	return false;
}

bool PruningVisitor::VisitNotNode(ASTNode **nodePtr)
{
	auto node = *nodePtr;

	ASTNode **subnodePtr = &(node->Nodes[0]);
	bool subKnown = VisitNode(subnodePtr);
	ASTNode *subNode = *subnodePtr;
	
	if (subKnown)
	{
		int value = !subNode->Value;
		delete subNode;
		node->Nodes.clear();
		SetKnown(node, value);
		return true;
	}
	else if (subNode->token.Type == TOKENTYPE_OPNOT) //!!x === x
	{
		ASTNode *subSubNode = subNode->Nodes[0];
		subNode->Nodes.clear();
		delete subNode;
		*subnodePtr = subNode = NULL;

		CopyTo(subSubNode, nodePtr);
		return subSubNode->Known;
	}
	return false;
}

bool PruningVisitor::VisitIDNode(ASTNode **nodePtr)
{
	return false;
}

bool PruningVisitor::VisitIntNumberNode(ASTNode **nodePtr)
{
	auto node = *nodePtr;

	node->Value = StringHelper_toInt(node->token.Value);
	node->Known = true;
	return true;
}

bool PruningVisitor::VisitTrueNode(ASTNode **nodePtr)
{
	auto node = *nodePtr;

	SetKnown(node, 1);
	return true;
}

bool PruningVisitor::VisitFalseNode(ASTNode **nodePtr)
{
	auto node = *nodePtr;

	SetKnown(node, 0);
	return true;
}

void PruningVisitor::CopyTo(ASTNode *srcptr, ASTNode **targetptr)
{
	(*targetptr)->Nodes.clear(); //to avoid delete sub nodes
	delete (*targetptr); 
	*targetptr = srcptr;
}

void PruningVisitor::SetKnown(ASTNode *node, int value)
{
	node->Value = value;
	node->token.Type = TOKENTYPE_INTNUMBER;
	node->token.Value = StringHelper_toString(value);
	node->token.IntValue = value;
	node->Known = true;
}