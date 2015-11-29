#include "stdafx.h"
#include "SyntaxError.h"


SyntaxError::SyntaxError()
{
}

SyntaxError::SyntaxError(std::string what) : what(what)
{}

SyntaxError::SyntaxError(std::string what, ptrdiff_t LineNumber, ptrdiff_t ColumnStart)
	: what(what), LineNumber(LineNumber), ColumnStart(ColumnStart)
{
	if (LineNumber < 0 || ColumnStart < 0) return;
	this->what = "Line " + StringHelper_toString(LineNumber) + ", Col " + StringHelper_toString(ColumnStart) + ": " + this->what;
}

SyntaxError::SyntaxError(std::string what, ptrdiff_t LineNumber, ptrdiff_t ColumnStart, ptrdiff_t ColumnEnd)
	: what(what), LineNumber(LineNumber), ColumnStart(ColumnStart), ColumnEnd(ColumnEnd)
{
	if (LineNumber < 0 || ColumnStart < 0) return;
	if (ColumnStart != ColumnEnd)
	{
		this->what = "Line " + StringHelper_toString(LineNumber) + ", Col " + StringHelper_toString(ColumnStart) + "~" + StringHelper_toString(ColumnEnd) + ": " + this->what;
	}
	else
	{
		this->what = "Line " + StringHelper_toString(LineNumber) + ", Col " + StringHelper_toString(ColumnStart) + ": " + this->what;
	}
}

SyntaxError::~SyntaxError()
{
}
