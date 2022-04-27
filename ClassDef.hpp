#pragma once

#include "pch.hpp"

class ClassDef
{
private:
	std::string name;
	std::vector<std::pair<std::string, std::string>> variables;
	std::vector<std::pair<std::string, Function>> methods;

public:
	ClassDef() = default;

	explicit ClassDef(const std::string& name, const std::string& body);

	[[nodiscard]] std::string getName() const
	{ return name; }
};