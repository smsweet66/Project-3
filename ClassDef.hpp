#pragma once

#include <string>
#include <vector>
#include "Function.hpp"

class ClassDef
{
private:
	std::string name;
	std::vector<std::pair<std::string, std::string>> variables;
	std::vector<Function> methods;

public:
	ClassDef() = default;

	explicit ClassDef(std::string  name, const std::string& body);

	bool containsVariable(const std::string& name) const;

	[[nodiscard]] bool containsMethod(const std::string& funcName, const std::vector<std::string> &parameters) const;

	Function& getMethod(const std::string& funcName, const std::vector<std::string> &parameters);

	[[nodiscard]] std::string getName() const
	{ return name; }
};