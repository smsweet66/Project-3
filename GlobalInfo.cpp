#include "GlobalInfo.hpp"

std::vector<std::unordered_map<std::string, Variable>> GlobalInfo::variables(1);
std::vector<std::unordered_map<std::string, ClassDef>> GlobalInfo::classes(1);
std::vector<std::unordered_map<std::string, std::list<Function>>> GlobalInfo::functions(1);

void GlobalInfo::addVariable(const Variable& var)
{
	for(auto& scope : variables)
	{
		if(scope.find(var.getName()) != scope.end())
			throw std::runtime_error("Variable " + var.getName() + " already exists");
	}

	variables.back()[var.getName()] = var;
}

void GlobalInfo::assignVariable(const std::string &name, std::string &line)
{
	for(auto& scope : variables)
	{
		if(scope.find(name) != scope.end())
		{
			scope[name] = Variable(line);
			return;
		}
	}

	throw std::runtime_error("Variable " + name + " does not exist");
}

void GlobalInfo::addClassDef(const ClassDef& classDef)
{
	for(auto& scope : classes)
		if(scope.find(classDef.getName()) != scope.end())
			throw std::runtime_error("Class " + classDef.getName() + " already defined");

	classes.back()[classDef.getName()] = classDef;
}

void GlobalInfo::addFunction(const Function& function)
{
	for(auto& scope : functions)
		if(scope.find(function.getName()) != scope.end())
			for(auto& func : scope[function.getName()])
				if(function == func)
					throw std::runtime_error("Function " + function.getName() + " already defined");

	functions.back()[function.getName()].push_back(function);
}

void GlobalInfo::increaseScope()
{
	variables.emplace_back();
	classes.emplace_back();
	functions.emplace_back();
}

void GlobalInfo::decreaseScope()
{
	variables.pop_back();
	classes.pop_back();
	functions.pop_back();
}

bool GlobalInfo::isVariable(const std::string &name)
{
	for(auto& scope : variables)
		if(scope.find(name) != scope.end())
			return true;

	return false;
}

Variable &GlobalInfo::getVariable(const std::string &name)
{
	for(auto& scope : variables)
		if(scope.find(name) != scope.end())
			return scope[name];

	throw std::runtime_error("Variable " + name + " does not exist");
}
