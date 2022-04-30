#include "GlobalInfo.hpp"

std::vector<std::unordered_map<std::string, Variable>> GlobalInfo::variables(1);
std::vector<std::unordered_map<std::string, ClassDef>> GlobalInfo::classes(1);
std::vector<std::unordered_map<std::string, std::list<Function>>> GlobalInfo::functions(1);

Logger GlobalInfo::logger = Logger("GlobalInfo");

void GlobalInfo::addVariable(Variable var)
{ variables.back()[var.getName()] = var; }

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

Function& GlobalInfo::getFunction(const std::string &name, const std::vector<std::string>& parameterTypes, int minScope)
{
	if(minScope == 0)
	{
		for(auto& scope : functions)
		{
			if(scope.find(name) != scope.end())
			{
				for(auto& func : scope[name])
					if(func.matches(name, parameterTypes))
						return func;
			}
		}
	}
	else
	{
		for(int i = minScope; i < functions.size(); i++)
		{
			if(functions[i].find(name) != functions[i].end())
			{
				for(auto& func : functions[i][name])
					if(func.matches(name, parameterTypes))
						return func;
			}
		}

		if(functions[0].find(name) != functions[0].end())
		{
			for(auto& func : functions[0][name])
				if(func.matches(name, parameterTypes))
					return func;
		}

		logger.error("Function " + name + " not found");
		exit(1);
	}
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

bool GlobalInfo::isVariable(const std::string &name, int minScope)
{
	if(minScope == 0)
	{
		for(auto &scope: variables)
			if(scope.find(name) != scope.end())
				return true;
	}
	else
	{
		for(int i = minScope; i < variables.size(); i++)
			if(variables[i].find(name) != variables[i].end())
				return true;

		if(variables[0].find(name) != variables[0].end())
			return true;
	}

	return false;
}

Variable &GlobalInfo::getVariable(const std::string &name, int minScope)
{
	if(minScope == 0)
	{
		for(auto &scope: variables)
			if(scope.find(name) != scope.end())
				return scope[name];
	}
	else
	{
		for(int i = minScope; i < variables.size(); i++)
			if(variables[i].find(name) != variables[i].end())
				return variables[i][name];

		if(variables[0].find(name) != variables[0].end())
			return variables[0][name];
	}

	throw std::runtime_error("Variable " + name + " does not exist");
}

ClassDef& GlobalInfo::getClassDef(const std::string &name, int minScope)
{
	if(minScope == 0)
	{
		for(auto &scope: classes)
			if(scope.find(name) != scope.end())
				return scope[name];
	}
	else
	{
		for(int i = minScope; i < classes.size(); i++)
			if(classes[i].find(name) != classes[i].end())
				return classes[i][name];

		if(classes[0].find(name) != classes[0].end())
			return classes[0][name];
	}

	throw std::runtime_error("Class " + name + " does not exist");
}

bool GlobalInfo::isClass(const std::string &name, int minScope)
{
	if(minScope == 0)
	{
		for(auto &scope: classes)
			if(scope.find(name) != scope.end())
				return true;
	}
	else
	{
		for(int i = minScope; i < classes.size(); i++)
			if(classes[i].find(name) != classes[i].end())
				return true;

		if(classes[0].find(name) != classes[0].end())
			return true;
	}

	return false;
}

bool GlobalInfo::isFunction(const std::string &name, int minScope)
{
	if(minScope == 0)
	{
		for(auto &scope: functions)
			if(scope.find(name) != scope.end())
				return true;
	}
	else
	{
		for(int i = minScope; i < functions.size(); i++)
			if(functions[i].find(name) != functions[i].end())
				return true;

		if(functions[0].find(name) != functions[0].end())
			return true;
	}

	return false;
}
