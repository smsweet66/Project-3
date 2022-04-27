#pragma once

#include "pch.hpp"

class Function
{
private:
	std::string name;
	std::vector<std::pair<std::string, std::string>> parameterInfo;
	std::string body;
	std::string returnType;

public:
	explicit Function(std::string &name, std::string &parameterInfo, std::string &body, std::string &returnType);

	[[nodiscard]] std::string getName() const
	{ return name; }

	/*
	 * Returns whether the function has the same parameter types as the given function
	 */
	bool operator==(const Function& other) const
	{ return parameterInfo == other.parameterInfo; }
};