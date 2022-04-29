#pragma once

#include <vector>
#include <unordered_map>
#include <list>
#include "Variable.hpp"
#include "ClassDef.hpp"

class GlobalInfo
{
private:
	static std::vector<std::unordered_map<std::string, Variable>> variables;
	static std::vector<std::unordered_map<std::string, ClassDef>> classes;
	static std::vector<std::unordered_map<std::string, std::list<Function>>> functions;

	static Logger logger;

public:
	static void addVariable(Variable var);

	static void assignVariable(const std::string& name, std::string &line);

	static Variable& getVariable(const std::string& name, int minScope = 0);

	static void addClassDef(const ClassDef& classDef);

	static ClassDef& getClassDef(const std::string& name, int minScope = 0);

	static void addFunction(const Function& function);

	static Function& getFunction(const std::string& name, const std::vector<std::string>& parameterTypes, int minScope = 0);

	static void increaseScope();

	static void decreaseScope();

	static size_t getScope()
	{ return variables.size(); }

	static bool isVariable(const std::string& name, int minScope = 0);

	static bool isClass(const std::string& name, int minScope = 0);

	static bool isFunction(const std::string& name, int minScope = 0);
};