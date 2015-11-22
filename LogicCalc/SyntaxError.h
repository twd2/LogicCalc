#pragma once

#include <string>

class SyntaxError
{
public:
	std::string what;

	SyntaxError();

	SyntaxError(std::string what) :what(what)
	{}

	~SyntaxError();
};

