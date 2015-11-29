#pragma once

enum TokenType
{
	TOKENTYPE_NOTTOKEN,
	TOKENTYPE_ID, // [$_A-Za-z][$_A-Za-z0-9]*
	TOKENTYPE_INTNUMBER, // [0-9]+
	TOKENTYPE_FLOATNUMBER, //[0-9]+\.[0-9]* | \.[0-9]+
	TOKENTYPE_CHARVALUE, // '(\[\0anrt'"]|[\s\S])'
	TOKENTYPE_STRINGVALUE, // "(\[\0anrt'"]|[\s\S]*)"
	TOKENTYPE_LROUNDBRACKET, // (
	TOKENTYPE_RROUNDBRACKET, // )
	TOKENTYPE_LSQUAREBRACKET, // [
	TOKENTYPE_RSQUAREBRACKET, // ]
	TOKENTYPE_LBRACE, // {
	TOKENTYPE_RBRACE, // }
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
	TOKENTYPE_OPLET, // =
	TOKENTYPE_OPEQU, // ==
	TOKENTYPE_OPNEQU, // !=
	TOKENTYPE_OPDOT, // .
	TOKENTYPE_OPCOMMA, // ,
	TOKENTYPE_OPCOLON, // :
	TOKENTYPE_OPSEMICOLON, // ;
	TOKENTYPE_TRUE, // true
	TOKENTYPE_FALSE, // false
	TOKENTYPE_FOR, // for
	TOKENTYPE_IF, // if
	TOKENTYPE_WHILE, // while
	TOKENTYPE_BREAK, // break
	TOKENTYPE_CONTINUE, // continue
	TOKENTYPE_ELSE, // else
	TOKENTYPE_RETURN, // return
	TOKENTYPE_WANDAI, // wandai
	TOKENTYPE_COMMENT, // //xxx /*xxx*/
	TOKENTYPE_EOF = -1
};

class Token
{
public:
	TokenType Type;
	int LexerID, ID; // ID for TOKENTYPE_ID
	std::string Value; // raw string / StringValue / CharValue
	int IntValue; // IntValue
	double FloatValue; // FloatValue

	ptrdiff_t LineNumber, ColumnStart, ColumnEnd;

	Token(TokenType type, std::string value)
		: Type(type), Value(value), LineNumber(-1), ColumnStart(-1), ColumnEnd(-1)
	{}

	Token(TokenType type, std::string value, ptrdiff_t lineNumber, ptrdiff_t columnStart, ptrdiff_t columnEnd) 
		: Type(type), Value(value), LineNumber(lineNumber), ColumnStart(columnStart), ColumnEnd(columnEnd)
	{}

	~Token();

	bool operator==(const Token &b);
	bool operator!=(const Token &b);

	static std::string Token::Name(TokenType);
};