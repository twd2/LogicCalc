#pragma once
class ASTNode
{
public:

	Token token;

	int Value = 0; //ֵ
	bool Known = false; //�Ƿ��Ѿ�֪����ֵ

	std::vector<ASTNode*> Nodes;

	ASTNode(Token token) : token(token)
	{}

	~ASTNode();

	bool operator==(const ASTNode&);
	bool operator!=(const ASTNode&);

private:


};

