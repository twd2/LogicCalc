#pragma once

enum TokenType
{
	TOKENTYPE_ID, // [$_A-Za-z][$_A-Za-z0-9]*
	TOKENTYPE_INTNUMBER, // [0-9]+
	TOKENTYPE_FLOATNUMBER, //[0-9]+\.[0-9]* | \.[0-9]+
	TOKENTYPE_LBRACKET, // (
	TOKENTYPE_RBRACKET, // )
	TOKENTYPE_OPNOT, // !
	TOKENTYPE_OPAND, // &&
	TOKENTYPE_OPOR, // ||
	TOKENTYPE_OPXOR, // ^^
	TOKENTYPE_OPBITNOT, // ~
	TOKENTYPE_OPBITAND, // &
	TOKENTYPE_OPBITOR, // |
	TOKENTYPE_OPBITXOR, // ^
	TOKENTYPE_OPIMP, // ->
	TOKENTYPE_OPDUALIMP, // <->
	TOKENTYPE_OPTAUIMP, // =>
	TOKENTYPE_OPADD, // +
	TOKENTYPE_OPSUB, // - 
	TOKENTYPE_OPMUL, // *
	TOKENTYPE_OPDIV, // /
	TOKENTYPE_OPMOD, // %
	TOKENTYPE_OPGT, // >
	TOKENTYPE_OPGTE, // >=
	TOKENTYPE_OPLT, // <
	TOKENTYPE_OPLTE, // <=
	TOKENTYPE_OPEQU, // ==
	TOKENTYPE_WANDAI, // wandai
	TOKENTYPE_EOF = -1
};

class Token
{
public:
	TokenType Type;
	std::string Value;
	int LexerID, ID; //ID for TOKENTYPE_ID
	int IntValue; //IntValue
	double FloatValue; //TODO: FloatValue

	Token(TokenType type, std::string value) : Type(type), Value(value)
	{}

	~Token();
};

std::string Token_toName(TokenType);