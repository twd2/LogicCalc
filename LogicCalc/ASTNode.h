#pragma once
class ASTNode
{
public:

	Token &token;

	int Value = 0; //ֵ
	bool Known = false; //�Ƿ��Ѿ�֪����ֵ

	std::vector<ASTNode*> nodes;

	ASTNode(Token &token) : token(token)
	{}

	~ASTNode();

private:


};

