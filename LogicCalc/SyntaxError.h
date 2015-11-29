#pragma once

#include "StringHelper.h"
#include <string>

class SyntaxError
{
public:
	std::string what;
	ptrdiff_t LineNumber = 1, ColumnStart = -1, ColumnEnd = -1;

	SyntaxError();

	SyntaxError(std::string what) : what(what)
	{}

	SyntaxError(std::string what, ptrdiff_t LineNumber, ptrdiff_t ColumnStart) 
		: what(what), LineNumber(LineNumber), ColumnStart(ColumnStart)
	{
		this->what = "Line " + StringHelper_toString(LineNumber) + ", Col " + StringHelper_toString(ColumnStart) + ": " + this->what;
	}

	SyntaxError(std::string what, ptrdiff_t LineNumber, ptrdiff_t ColumnStart, ptrdiff_t ColumnEnd)
		: what(what), LineNumber(LineNumber), ColumnStart(ColumnStart), ColumnEnd(ColumnEnd)
	{
		if (ColumnStart != ColumnEnd)
		{
			this->what = "Line " + StringHelper_toString(LineNumber) + ", Col " + StringHelper_toString(ColumnStart) + "~" + StringHelper_toString(ColumnEnd) + ": " + this->what;
		}
		else
		{
			this->what = "Line " + StringHelper_toString(LineNumber) + ", Col " + StringHelper_toString(ColumnStart) + ": " + this->what;
		}
	}

	~SyntaxError();
};

