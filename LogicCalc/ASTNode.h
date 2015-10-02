#pragma once
class ASTNode
{
public:

	Token token;

	int Value = 0; //值
	bool Known = false; //是否已经知道了值

	std::vector<ASTNode*> nodes;

	/*ASTNode(Token &token) : token(token)
	{}*/

	ASTNode(Token token) : token(token)
	{}

	~ASTNode();

private:


};

