#pragma once

#include <vector>
#include <sstream>
#include <unordered_map>
#include "Variable.hpp"

/**
 * Splits a string at spaces into a vector of strings.
 * Keeps sections surrounded by quotes, parenthesis, and square brackets together.
 * @param str
 * @return a vector of the substrings
 */
std::vector<std::string> split(const std::string& str)
{
	std::vector<std::string> result;
	int start = 0;
	for(int i = 0; i < str.size(); ++i)
	{
		if(str[i] == ' ')
			continue;
		else if(str[i] == '\"')
		{
			start = i;
			i++;
			while(str[i] != '\"')
				i++;

			result.push_back(str.substr(start, i + 1 - start));
		}
		else if(str[i] == '(')
		{
			start = i;
			int depth = 1;
			i++;
			while(depth > 0)
			{
				if(str[i] == '(')
					depth++;
				else if(str[i] == ')')
					depth--;
				i++;
			}

			i--;
			result.push_back(str.substr(start, i + 1 - start));
		}
		else if(str[i] == '[')
		{
			start = i;
			int depth = 1;
			i++;
			while(depth > 0)
			{
				if(str[i] == '[')
					depth++;
				else if(str[i] == ']')
					depth--;
				i++;
			}
			i--;
			result.push_back(str.substr(start, i + 1 - start));
		}
		else
		{
			start = i;
			while(i < str.size() && str[i] != ' ')
				i++;

			result.push_back(str.substr(start, i - start));
		}
	}

	return result;
}

class Function
{
private:
	std::string name;
	std::vector<std::pair<std::string, std::string>> parameterInfo;
	std::string body;
	std::string returnType;

public:
	explicit Function(const std::string& functionInfo)
	{

	}

	[[nodiscard]] std::string getName() const
	{ return name; }

	/*
	 * Returns whether the function has the same parameter types as the given function
	 */
	bool operator==(const Function& other) const
	{ return parameterInfo == other.parameterInfo; }
};

class ClassDef
{
private:
	std::string name;
	std::vector<std::pair<std::string, std::string>> variables;
	std::vector<std::pair<std::string, Function>> methods;

public:
	//TODO needs proper input reading
	explicit ClassDef(const std::string& input)
	{

	}

	[[nodiscard]] std::string getName() const
	{ return name; }
};