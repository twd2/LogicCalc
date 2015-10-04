#pragma once

#include <string>
#include <vector>
#include <map>

class IdGenerator
{
public:

	std::map<std::string, int> Ids;

	IdGenerator();
	~IdGenerator();

	std::vector<std::string> GetKeys();
	int GetID(std::string&);

private:

	int count = 0;
};
