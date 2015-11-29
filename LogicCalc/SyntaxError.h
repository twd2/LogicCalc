#pragma once

#include "StringHelper.h"
#include <string>

class SyntaxError
{
public:
	std::string what;
	ptrdiff_t LineNumber = 1, ColumnStart = -1, ColumnEnd = -1;

	SyntaxError();
	SyntaxError(std::string);
	SyntaxError(std::string, ptrdiff_t, ptrdiff_t);
	SyntaxError(std::string, ptrdiff_t, ptrdiff_t, ptrdiff_t);

	~SyntaxError();
};

