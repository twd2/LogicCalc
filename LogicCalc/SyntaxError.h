#pragma once

#include "StringHelper.h"
#include <string>

class SyntaxError
{
public:
	std::string what;
	ptrdiff_t LineNumber = 1, Column = -1;

	SyntaxError();

	SyntaxError(std::string what) : what(what)
	{}

	SyntaxError(std::string what, ptrdiff_t LineNumber, ptrdiff_t Column) 
		: what(what), LineNumber(LineNumber), Column(Column)
	{
		this->what = "Line " + StringHelper_toString(LineNumber) + ", Col " + StringHelper_toString(Column) + ": " + this->what;
	}

	~SyntaxError();
};

