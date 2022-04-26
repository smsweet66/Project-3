#pragma once

#include "pch.hpp"

class GlobalInfo
{
private:
	std::unordered_map<std::string, Variable&> variables;
	std::unordered_map<std::string, ClassDef&> classes;
	std::unordered_map<std::string, std::list<Function>> functions;

public:
	GlobalInfo() = default;

	void addVariable(std::string line, bool constant);

	void addClassDef(const std::string& line);

	void addFunction(const std::string& line);

	bool containsVariable(const std::string& name)
	{ return variables.find(name) != variables.end(); }
};