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
bool PruningVisitor::VisitNode(ASTNode **node)
{
	switch ((*node)->token.Type)
	{
	case TOKENTYPE_ID:
		return VisitIDNode(node);
		break;
	case TOKENTYPE_NUMBER:
		return VisitNumberNode(node);
		break;
	case TOKENTYPE_OPADD:
		break;
	case TOKENTYPE_OPAND:
		return VisitAndNode(node);
		break;
	case TOKENTYPE_OPBITAND:
		return VisitBitandNode(node);
		break;
	case TOKENTYPE_OPBITNOT:
		return VisitBitnotNode(node);
		break;
	case TOKENTYPE_OPBITOR:
		return VisitBitorNode(node);
		break;
	case TOKENTYPE_OPBITXOR:
		return VisitBitxorNode(node);
		break;
	case TOKENTYPE_OPDIV:
		break;
	case TOKENTYPE_OPDUALIMP:
		return VisitDualimpNode(node);
		break;
	case TOKENTYPE_OPEQU:
		break;
	case TOKENTYPE_OPGT:
		break;
	case TOKENTYPE_OPGTE:
		break;
	case TOKENTYPE_OPIMP:
		return VisitImpNode(node);
		break;
	case TOKENTYPE_OPLT:
		break;
	case TOKENTYPE_OPLTE:
		break;
	case TOKENTYPE_OPMOD:
		break;
	case TOKENTYPE_OPMUL:
		break;
	case TOKENTYPE_OPNOT:
		return VisitNotNode(node);
		break;
	case TOKENTYPE_OPOR:
		return VisitOrNode(node);
		break;
	case TOKENTYPE_OPSUB:
		break;
	case TOKENTYPE_OPTAUIMP:
		break;
	case TOKENTYPE_OPXOR:
		return VisitXorNode(node);
		break;
	default:
		break;
	}
	return false;
}

bool PruningVisitor::VisitDualimpNode(ASTNode **nodeptr)
{
	ASTNode *node = *nodeptr;

	ASTNode **leftptr, **rightptr;
	leftptr = &(node->nodes[0]);
	rightptr = &(node->nodes[1]);

	bool leftKnown = VisitNode(leftptr),
		rightKnown = VisitNode(rightptr);

	ASTNode *left = *leftptr,
		*right = *rightptr;

	if (leftKnown && rightKnown)
	{
		bool value = left->Value == right->Value;

		delete left;
		*leftptr = left = NULL;
		delete right;
		*rightptr = right = NULL;

		node->nodes.clear();
		SetKnown(node, value);
		return true;
	}
	else if (CompareVisitor::Compare(left, right)) //P<->P === 1
	{
		delete left;
		*leftptr = left = NULL;
		delete right;
		*rightptr = right = NULL;

		node->nodes.clear();
		SetKnown(node, 1);
		return true;
	}
	else if ((leftKnown && left->Value == 0)) //0<->x === !x
	{
		delete left;
		*leftptr = left = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPNOT, "!"));
		newNode->nodes.push_back(right);

		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodeptr);

		return newKnown;
	}
	else if (leftKnown && left->Value == 1) //1<->x === x
	{
		delete left;
		*leftptr = left = NULL;

		CopyTo(right, nodeptr);

		return rightKnown;
	}
	else if (rightKnown && right->Value == 0) //x<->0 === !x
	{
		delete right;
		*rightptr = right = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPNOT, "!"));
		newNode->nodes.push_back(left);

		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodeptr);

		return newKnown;
	}
	else if (rightKnown && right->Value == 1) //x<->1 === x
	{
		delete right;
		*rightptr = right = NULL;

		CopyTo(left, nodeptr);

		return leftKnown;
	}

	return false;
}

bool PruningVisitor::VisitImpNode(ASTNode **nodeptr)
{
	ASTNode *node = *nodeptr;

	ASTNode **leftptr, **rightptr;
	leftptr = &(node->nodes[0]);
	rightptr = &(node->nodes[1]);

	bool leftKnown = VisitNode(leftptr),
		rightKnown = VisitNode(rightptr);

	ASTNode *left = *leftptr,
		*right = *rightptr;

	//P->Q === !P||Q
	ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPOR, "||"));

	ASTNode *newSubNode = new ASTNode(Token(TOKENTYPE_OPNOT, "!"));
	newSubNode->nodes.push_back(left);

	newNode->nodes.push_back(newSubNode);
	newNode->nodes.push_back(right);

	bool newKnown = VisitNode(&newNode);

	CopyTo(newNode, nodeptr);

	return newKnown;
}

bool PruningVisitor::VisitOrNode(ASTNode **nodeptr)
{
	ASTNode *node = *nodeptr;

	ASTNode **leftptr, **rightptr;
	leftptr = &(node->nodes[0]);
	rightptr = &(node->nodes[1]);

	bool leftKnown = VisitNode(leftptr),
		rightKnown = VisitNode(rightptr);

	ASTNode *left = *leftptr,
		*right = *rightptr;

	if ((leftKnown && left->Value == 1) || (rightKnown && right->Value == 1))
	{
		delete left;
		*leftptr = left = NULL;
		delete right;
		*rightptr = right = NULL;
		node->nodes.clear();
		SetKnown(node, 1);
		return true;
	}
	else if (leftKnown && left->Value == 0)
	{
		if (rightKnown && right->Value == 0)
		{
			delete left;
			*leftptr = left = NULL;
			delete right;
			*rightptr = right = NULL;
			node->nodes.clear();
			SetKnown(node, 0);
			return true;
		}
		else //!rightKnown
		{
			delete left;
			*leftptr = left = NULL;
			CopyTo(right, nodeptr);
			return false;
		}
	}
	else if (rightKnown && right->Value == 0) //!leftHasValue
	{
		delete right;
		*rightptr = right = NULL;
		CopyTo(left, nodeptr);
		return false;
	}
	else //!leftKnown && !rightKnown
	{
		if (CompareVisitor::Compare(left, right)) //P||P===P
		{
			delete right;
			*rightptr = right = NULL;
			CopyTo(left, nodeptr);
			return false;
		}
		else if (left->token.Type == TOKENTYPE_OPNOT &&
			CompareVisitor::Compare(left->nodes[0], right)) //!P||P===1
		{
			delete left;
			*leftptr = left = NULL;
			delete right;
			*rightptr = right = NULL;
			node->nodes.clear();
			SetKnown(node, 1);
			return true;
		}
		else if (right->token.Type == TOKENTYPE_OPNOT &&
			CompareVisitor::Compare(right->nodes[0], left)) //P||!P===1
		{
			delete left;
			*leftptr = left = NULL;
			delete right;
			*rightptr = right = NULL;
			node->nodes.clear();
			SetKnown(node, 1);
			return true;
		}
		else if (left->token.Type == TOKENTYPE_OPAND &&
			(CompareVisitor::Compare(left->nodes[0], right) ||
			CompareVisitor::Compare(left->nodes[1], right))) //(P&&Q)||P===P, (Q&&P)||P===P
		{
			delete left;
			*leftptr = left = NULL;
			CopyTo(right, nodeptr);
			return false;
		}
		else if (right->token.Type == TOKENTYPE_OPAND &&
			(CompareVisitor::Compare(right->nodes[0], left) ||
			CompareVisitor::Compare(right->nodes[1], left))) //P||(P&&Q)===P, P||(Q&&P)===P
		{
			delete right;
			*rightptr = right = NULL;
			CopyTo(left, nodeptr);
			return false;
		}
	}
	return false;
}

bool PruningVisitor::VisitXorNode(ASTNode **nodeptr)
{
	ASTNode *node = *nodeptr;

	ASTNode **leftptr, **rightptr;
	leftptr = &(node->nodes[0]);
	rightptr = &(node->nodes[1]);

	bool leftKnown = VisitNode(leftptr),
		rightKnown = VisitNode(rightptr);

	ASTNode *left = *leftptr,
		*right = *rightptr;

	if (leftKnown && rightKnown)
	{
		bool value = left->Value != right->Value;
		delete left;
		*leftptr = left = NULL;
		delete right;
		*rightptr = right = NULL;
		node->nodes.clear();
		SetKnown(node, value);
		return true;
	}
	else if (CompareVisitor::Compare(left, right)) //P^^P === 0
	{
		delete left;
		*leftptr = left = NULL;
		delete right;
		*rightptr = right = NULL;
		node->nodes.clear();
		SetKnown(node, 0);
		return true;
	}
	else if ((leftKnown && left->Value == 1)) //1^^x === !x
	{
		delete left;
		*leftptr = left = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPNOT, "!"));
		newNode->nodes.push_back(right);

		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodeptr);

		return newKnown;
	}
	else if (leftKnown && left->Value == 0) //0^x === x
	{
		delete left;
		*leftptr = left = NULL;

		CopyTo(right, nodeptr);

		return rightKnown;
	}
	else if (rightKnown && right->Value == 1) //x^1 === !x
	{
		delete right;
		*rightptr = right = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPNOT, "!"));
		newNode->nodes.push_back(left);

		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodeptr);

		return newKnown;
	}
	else if (rightKnown && right->Value == 0) //x^0 === x
	{
		delete right;
		*rightptr = right = NULL;

		CopyTo(left, nodeptr);

		return leftKnown;
	}
	return false;
}

bool PruningVisitor::VisitAndNode(ASTNode **nodeptr)
{
	ASTNode *node = *nodeptr;

	ASTNode **leftptr, **rightptr;
	leftptr = &(node->nodes[0]);
	rightptr = &(node->nodes[1]);

	bool leftKnown = VisitNode(leftptr),
		rightKnown = VisitNode(rightptr);

	ASTNode *left = *leftptr,
		*right = *rightptr;

	if ((leftKnown && left->Value == 0) || (rightKnown && right->Value == 0)) // 0&&P === 0, P&&0 === 0
	{
		delete left;
		*leftptr = left = NULL;
		delete right;
		*rightptr = right = NULL;
		node->nodes.clear();
		SetKnown(node, 0);
		return true;
	}
	else if (leftKnown && left->Value == 1)
	{
		if (rightKnown && right->Value == 1) //1&&1 === 1
		{
			delete left;
			*leftptr = left = NULL;
			delete right;
			*rightptr = right = NULL;
			node->nodes.clear();
			SetKnown(node, 1);
			return true;
		}
		else  //1&&P === P
		{
			delete left;
			*leftptr = left = NULL;
			CopyTo(right, nodeptr);
			return false;
		}
	}
	else if (rightKnown && right->Value == 1) //P&&1 === P
	{
		delete right;
		*rightptr = right = NULL;
		CopyTo(left, nodeptr);
		return false;
	}
	else
	{
		if (CompareVisitor::Compare(left, right)) //P&&P===P
		{
			delete right;
			*rightptr = right = NULL;
			CopyTo(left, nodeptr);
			return false;
		}
		else if (left->token.Type == TOKENTYPE_OPNOT &&
			CompareVisitor::Compare(left->nodes[0], right)) //!P&&P===0
		{
			delete left;
			*leftptr = left = NULL;
			delete right;
			*rightptr = right = NULL;
			node->nodes.clear();
			SetKnown(node, 0);
			return true;
		}
		else if (right->token.Type == TOKENTYPE_OPNOT &&
			CompareVisitor::Compare(right->nodes[0], left)) //P&&!P===0
		{
			delete left;
			*leftptr = left = NULL;
			delete right;
			*rightptr = right = NULL;
			node->nodes.clear();
			SetKnown(node, 0);
			return true;
		}
		else if (left->token.Type == TOKENTYPE_OPOR &&
			(CompareVisitor::Compare(left->nodes[0], right) ||
			CompareVisitor::Compare(left->nodes[1], right))) //(P||Q)&&P===P, (Q||P)&&P===P
		{
			delete left;
			*leftptr = left = NULL;
			CopyTo(right, nodeptr);
			return false;
		}
		else if (right->token.Type == TOKENTYPE_OPOR &&
			(CompareVisitor::Compare(right->nodes[0], left) ||
			CompareVisitor::Compare(right->nodes[1], left))) //P&&(P||Q)===P, P&&(Q||P)===P
		{
			delete right;
			*rightptr = right = NULL;
			CopyTo(left, nodeptr);
			return false;
		}
	}
	return false;
}

bool PruningVisitor::VisitBitnotNode(ASTNode **nodeptr)
{
	ASTNode *node = *nodeptr;

	ASTNode **subNodePtr = &(node->nodes[0]);
	bool subKnown = VisitNode(subNodePtr);
	ASTNode *subNode = *subNodePtr;
	if (subKnown)
	{
		int value = ~subNode->Value;
		delete subNode;
		node->nodes.clear();
		SetKnown(node, value);
		return true;
	}
	else if (subNode->token.Type == TOKENTYPE_OPBITNOT) //~~x === x
	{
		ASTNode *subSubNode = subNode->nodes[0];
		subNode->nodes.clear();
		delete subNode;
		*subNodePtr = subNode = NULL;

		CopyTo(subSubNode, nodeptr);
		return subSubNode->Known;
	}
	return false;
}

bool PruningVisitor::VisitBitorNode(ASTNode **nodeptr)
{
	ASTNode *node = *nodeptr;

	ASTNode **leftptr, **rightptr;
	leftptr = &(node->nodes[0]);
	rightptr = &(node->nodes[1]);
	bool leftKnown = VisitNode(leftptr),
		rightKnown = VisitNode(rightptr);

	ASTNode *left = *leftptr,
		*right = *rightptr;

	if ((leftKnown && left->Value == 0xFFFFFFFF) || (rightKnown && right->Value == 0xFFFFFFFF))
	{
		delete left;
		delete right;
		node->nodes.clear();
		SetKnown(node, 0xFFFFFFFF);
		return true;
	}
	else if (leftKnown && left->Value == 0)
	{
		delete left;
		CopyTo(right, nodeptr);
		return false;
	}
	else if (rightKnown && right->Value == 0)
	{
		delete right;
		CopyTo(left, nodeptr);
		return false;
	}
	return false;
}

bool PruningVisitor::VisitBitxorNode(ASTNode **nodeptr)
{
	ASTNode *node = *nodeptr;

	ASTNode **leftptr, **rightptr;
	leftptr = &(node->nodes[0]);
	rightptr = &(node->nodes[1]);
	bool leftKnown = VisitNode(leftptr),
		rightKnown = VisitNode(rightptr);

	ASTNode *left = *leftptr,
		*right = *rightptr;

	if ((leftKnown && left->Value == 0xFFFFFFFF)) //1^x === ~x
	{
		delete left;
		*leftptr = left = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPBITNOT, "~"));
		newNode->nodes.push_back(right);
		
		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodeptr);

		return newKnown;
	}
	else if (leftKnown && left->Value == 0) //0^x === x
	{
		delete left;
		*leftptr = left = NULL;

		CopyTo(right, nodeptr);

		return rightKnown;
	}
	else if (rightKnown && right->Value == 0xFFFFFFFF) //x^1 === ~x
	{
		delete right;
		*rightptr = right = NULL;

		ASTNode *newNode = new ASTNode(Token(TOKENTYPE_OPBITNOT, "~"));
		newNode->nodes.push_back(left);

		bool newKnown = VisitNode(&newNode);
		CopyTo(newNode, nodeptr);

		return newKnown;
	}
	else if (rightKnown && right->Value == 0) //x^0 === x
	{
		delete right;
		*rightptr = right = NULL;

		CopyTo(left, nodeptr);

		return leftKnown;
	}
	return false;
}

bool PruningVisitor::VisitBitandNode(ASTNode **nodeptr)
{
	ASTNode *node = *nodeptr;

	ASTNode **leftptr, **rightptr;
	leftptr = &(node->nodes[0]);
	rightptr = &(node->nodes[1]);
	bool leftKnown = VisitNode(leftptr),
		rightKnown = VisitNode(rightptr);

	ASTNode *left = *leftptr,
		*right = *rightptr;

	if ((leftKnown && left->Value == 0) || (rightKnown && right->Value == 0))
	{
		delete left;
		*leftptr = left = NULL;
		delete right;
		*rightptr = right = NULL;

		node->nodes.clear();
		SetKnown(node, 0);
		return true;
	}
	else if (leftKnown && left->Value == 0xFFFFFFFF)
	{
		delete left;
		*leftptr = left = NULL;

		CopyTo(right, nodeptr);
		return rightKnown;
	}
	else if (rightKnown && right->Value == 0xFFFFFFFF) 
	{
		delete right;
		*rightptr = right = NULL;

		CopyTo(left, nodeptr);
		return leftKnown;
	}
	return false;
}

bool PruningVisitor::VisitNotNode(ASTNode **nodeptr)
{
	auto node = *nodeptr;

	ASTNode **subNodePtr = &(node->nodes[0]);
	bool subKnown = VisitNode(subNodePtr);
	ASTNode *subNode = *subNodePtr;
	
	if (subKnown)
	{
		int value = !subNode->Value;
		delete subNode;
		node->nodes.clear();
		SetKnown(node, value);
		return true;
	}
	else if (subNode->token.Type == TOKENTYPE_OPNOT) //!!x === x
	{
		ASTNode *subSubNode = subNode->nodes[0];
		subNode->nodes.clear();
		delete subNode;
		*subNodePtr = subNode = NULL;

		CopyTo(subSubNode, nodeptr);
		return subSubNode->Known;
	}
	return false;
}

bool PruningVisitor::VisitIDNode(ASTNode **nodeptr)
{
	return false;
}

bool PruningVisitor::VisitNumberNode(ASTNode **nodeptr)
{
	auto node = *nodeptr;

	node->Value = StringHelper_toInt(node->token.Value);
	node->Known = true;
	return true;
}

void PruningVisitor::CopyTo(ASTNode *srcptr, ASTNode **targetptr)
{
	(*targetptr)->nodes.clear(); //to avoid delete sub nodes
	delete (*targetptr); 
	*targetptr = srcptr;
}

void PruningVisitor::SetKnown(ASTNode *node, int value)
{
	node->Value = value;
	node->token.Type = TOKENTYPE_NUMBER;
	node->token.Value = StringHelper_toString(value);
	node->Known = true;
}