#pragma once
class ASTNode
{
public:

	Token &token;
	std::vector<ASTNode*> nodes;

	ASTNode(Token &token) : token(token)
	{}

	~ASTNode();

private:


};

