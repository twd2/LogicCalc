#pragma once

enum NodeType
{
	NODETYPE_POSITIVE, // positive
	NODETYPE_NEGATIVE // negative
};

class ASTNode
{
public:

	Token token;

	NodeType Type;

	int Value = 0; //值
	bool Known = false; //是否已经知道了值

	std::vector<ASTNode*> Nodes;

	ASTNode(Token token) : token(token)
	{}

	~ASTNode();

	bool operator==(const ASTNode&);
	bool operator!=(const ASTNode&);

private:


};

