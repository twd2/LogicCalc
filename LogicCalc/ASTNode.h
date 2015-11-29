#pragma once

enum NodeType
{
	NODETYPE_POSITIVE, // positive
	NODETYPE_NEGATIVE, // negative
	// TODO: types
};

class ASTNode
{
public:

	Token token;

	NodeType Type;

	int Value = 0; // known value
	bool Known = false; // value is known?

	std::vector<ASTNode*> Nodes;

	ASTNode(Token token) : token(token)
	{}

	~ASTNode();

	bool operator==(const ASTNode&);
	bool operator!=(const ASTNode&);

private:


};

