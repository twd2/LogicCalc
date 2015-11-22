#pragma once
class RuntimeError
{
public:
	std::string what;

	RuntimeError();

	RuntimeError(std::string what) :what(what)
	{}

	~RuntimeError();
};

