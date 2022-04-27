#pragma once

#include "pch.hpp"

class GlobalInfo
{
private:
	std::vector<std::unordered_map<std::string, Variable>> variables;
	std::vector<std::unordered_map<std::string, ClassDef>> classes;
	std::vector<std::unordered_map<std::string, std::list<Function>>> functions;

public:
	GlobalInfo()
	: variables(1), classes(1), functions(1) {}

	void addVariable(const Variable& var);

	void addClassDef(const ClassDef& classDef);

	void addFunction(const Function& function);

	void increaseScope();

	void decreaseScope();
};