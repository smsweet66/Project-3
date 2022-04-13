#pragma once

#include <vector>
#include <sstream>
#include <unordered_map>
#include "Variable.hpp"

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