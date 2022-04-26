#pragma once

#include "pch.hpp"

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