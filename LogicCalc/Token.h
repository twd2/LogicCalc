#pragma once

enum TokenType
{
	ID, // P, Q
	NUMBER, // 0, 1, 2~9
	LBRACKET, // (
	RBRACKET, // )
	OPNOT, // !
	OPAND, // &&
	OPOR, // ||
	OPXOR, // ^^
	OPBITNOT, // ~
	OPBITAND, // &
	OPBITOR, // |
	OPBITXOR, // ^
	OPIMP, // ->
	OPDUALIMP, // <->
	OPTAUIMP, // =>
	OPADD, // +
	OPSUB, // - 
	OPMUL, // *
	OPDIV, // /
	OPMOD, // %
	OPGT, // >
	OPGTE, // >=
	OPLT, // <
	OPLTE, // <=
	OPEQU, // ==
	_EOF = 0x7FFFFFFF
};

class Token
{
public:
	TokenType Type;
	std::string Value;

	Token(TokenType type, std::string value) : Type(type), Value(value)
	{}

	~Token();
};
