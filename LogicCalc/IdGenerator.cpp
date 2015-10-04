#include "stdafx.h"
#include "IdGenerator.h"


IdGenerator::IdGenerator()
{

}


IdGenerator::~IdGenerator()
{

}

bool myCompare(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b)
{
	return a.second < b.second;
}

std::vector<std::string> IdGenerator::GetKeys()
{
	std::vector<std::pair<std::string, int> > temp;
	temp.assign(Ids.begin(), Ids.end());
	std::sort(temp.begin(), temp.end(), myCompare);

	std::vector<std::string> keys;
	for each (auto pair in temp)
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
