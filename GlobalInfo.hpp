#pragma once

#include "pch.hpp"

class GlobalInfo
{
private:
	static std::vector<std::unordered_map<std::string, Variable>> variables;
	static std::vector<std::unordered_map<std::string, ClassDef>> classes;
	static std::vector<std::unordered_map<std::string, std::list<Function>>> functions;

public:
	static void addVariable(const Variable& var);

	static void assignVariable(const std::string& name, std::string &line);

	static Variable& getVariable(const std::string& name, int minScope = 0);

	static void addClassDef(const ClassDef& classDef);

	static ClassDef& getClassDef(const std::string& name, int minScope = 0);

	static void addFunction(const Function& function);

	static void increaseScope();

	static void decreaseScope();

	static int getScope()
	{ return variables.size(); }

	static bool isVariable(const std::string& name, int minScope = 0);
};