#include "stdafx.h"
#include "Token.h"

Token::~Token()
{

}

bool Token::operator==(Token & b)
{
	return Value == b.Value && Type == b.Type;
}

bool Token::operator!=(Token & b)
{
	return !(*this == b);
}

std::string Token_toName(TokenType type)
{
	static std::map<TokenType, std::string> names;
	if (names[TOKENTYPE_EOF] == "")
	{
		//init
		names[TOKENTYPE_NOTTOKEN] = "?";
		names[TOKENTYPE_ID] = "ID";
		names[TOKENTYPE_INTNUMBER] = "INTNUMBER";
		names[TOKENTYPE_FLOATNUMBER] = "FLOATNUMBER";
		names[TOKENTYPE_LBRACKET] = "LBRACKET";
		names[TOKENTYPE_RBRACKET] = "RBRACKET";
		names[TOKENTYPE_OPNOT] = "OPNOT";
		names[TOKENTYPE_OPAND] = "OPAND";
		names[TOKENTYPE_OPOR] = "OPOR";
		names[TOKENTYPE_OPXOR] = "OPXOR";
		names[TOKENTYPE_OPBITNOT] = "OPBITNOT";
		names[TOKENTYPE_OPBITAND] = "OPBITAND";
		names[TOKENTYPE_OPBITOR] = "OPBITOR";
		names[TOKENTYPE_OPBITXOR] = "OPBITXOR";
		names[TOKENTYPE_OPIMP] = "OPIMP";
		names[TOKENTYPE_OPDUALIMP] = "OPDUALIMP";
		names[TOKENTYPE_OPTAUIMP] = "OPTAUIMP";
		names[TOKENTYPE_OPADD] = "OPADD";
		names[TOKENTYPE_OPSUB] = "OPSUB";
		names[TOKENTYPE_OPMUL] = "OPMUL";
		names[TOKENTYPE_OPDIV] = "OPDIV";
		names[TOKENTYPE_OPMOD] = "OPMOD";
		names[TOKENTYPE_OPGT] = "OPGT";
		names[TOKENTYPE_OPGTE] = "OPGTE";
		names[TOKENTYPE_OPLT] = "OPLT";
		names[TOKENTYPE_OPLTE] = "OPLTE";
		names[TOKENTYPE_OPEQU] = "OPEQU";
		names[TOKENTYPE_OPDOT] = "OPDOT";
		names[TOKENTYPE_TRUE] = "TRUE";
		names[TOKENTYPE_FALSE] = "FALSE";
		names[TOKENTYPE_WANDAI] = "WANDAI";
		names[TOKENTYPE_EOF] = "EOF";
	}

	return names[type];
}