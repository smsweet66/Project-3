#pragma once

#include <vector>
#include <unordered_map>
#include <list>
#include "Variable.hpp"
#include "Function.hpp"

class GlobalInfo
{
private:
	static std::vector<std::unordered_map<std::string, Variable>> variables;
	static std::vector<std::unordered_map<std::string, std::list<Function>>> functions;
	static std::array<std::string, 15> reservedWords;
	static std::array<std::string, 15> operators;

	static Logger logger;

public:
	static void addVariable(const Variable& var);

	static Variable& getVariable(const std::string& name, int minScope = 0);

	static void addFunction(const Function& function);

	static Function& getFunction(const std::string& name, const std::vector<std::string>& parameterTypes, int minScope = 0);

	static void increaseScope();

	static void decreaseScope();

	static size_t getScope()
	{ return variables.size() - 1; }

	static bool isVariable(const std::string& name, int minScope = 0);

	static bool isFunction(const std::string& name, int minScope = 0);
};