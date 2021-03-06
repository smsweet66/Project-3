#pragma once

#include <string>
#include <vector>
#include "Variable.hpp"

class Function
{
private:
	std::string name;
	std::vector<std::pair<std::string, std::string>> parameterInfo;
	std::string body;
	std::string returnType;
	static Logger logger;

public:
	explicit Function(std::string &name, std::string &parameterInfo, std::string &body, std::string &returnType);

	[[nodiscard]] bool matches(const std::string &name, std::vector<std::string> parameterTypes) const;

	std::string run(std::vector<Variable> parameterValues) const;

	[[nodiscard]] std::string getName() const
	{ return name; }

	/*
	 * Returns whether the function has the same parameter types as the given function
	 */
	bool operator==(const Function& other) const
	{ return parameterInfo == other.parameterInfo; }
};