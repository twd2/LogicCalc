#pragma once

#include <string>

class SyntaxError
{
public:
	std::string why;

	SyntaxError();

	SyntaxError(std::string why) :why(why)
	{}

	~SyntaxError();
};

