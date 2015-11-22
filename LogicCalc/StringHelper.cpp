#include "stdafx.h"
#include "StringHelper.h"

std::string StringHelper_toString(int a)
{
	std::stringstream ss;
	ss << a;
	return ss.str();
}

int StringHelper_toInt(std::string a)
{
	std::stringstream ss;
	ss << a;
	int i;
	ss >> i;
	return i;
}

double StringHelper_toDouble(std::string a)
{
	std::stringstream ss;
	ss << a;
	double d;
	ss >> d;
	return d;
}