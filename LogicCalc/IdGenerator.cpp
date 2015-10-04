#include "stdafx.h"
#include "IdGenerator.h"


IdGenerator::IdGenerator()
{

}


IdGenerator::~IdGenerator()
{

}

std::vector<std::string> IdGenerator::GetKeys()
{
	std::vector<std::string> keys;
	for each (auto pair in Ids)
	{
		keys.push_back(pair.first);
	}
	return keys;
}

int IdGenerator::GetID(std::string &str)
{
	int v = Ids[str];
	if (v == 0)
	{
		Ids[str] = ++count;
		return count - 1;
	}
	else
	{
		return Ids[str] - 1;
	}
}
