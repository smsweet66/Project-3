#pragma once

#include "pch.hpp"

class ClassDef
{
private:
	std::string name;
	std::vector<std::pair<std::string, std::string>> variables;
	std::vector<Function> methods;

public:
	ClassDef() = default;

	explicit ClassDef(std::string  name, const std::string& body);

	[[nodiscard]] bool containsMethod(const std::string& funcName, const std::vector<std::string> &parameters) const;

	[[nodiscard]] std::string getName() const
	{ return name; }
};