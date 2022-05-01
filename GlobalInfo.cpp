#include "GlobalInfo.hpp"

std::vector<std::unordered_map<std::string, Variable>> GlobalInfo::variables(1);
std::vector<std::unordered_map<std::string, std::list<Function>>> GlobalInfo::functions(1);
std::array<std::string, 15> GlobalInfo::reservedWords =
		{"num", "string", "bool", "if", "else", "while", "true", "false", "var", "def", "return", "break", "continue", "print", "class"};

std::array<std::string, 15> GlobalInfo::operators =
		{"+", "-", "*", "/", "=", "==", "!=", ">", "<", ">=", "<=", "++", "--", "&&", "||"};

Logger GlobalInfo::logger = Logger("GlobalInfo");

void GlobalInfo::addVariable(const Variable& var)
{
	if(std::find(reservedWords.begin(), reservedWords.end(), var.getName()) != reservedWords.end())
	{
		logger.error("Reserved word " + var.getName() + " cannot be used as variable name");
		exit(1);
	}
	else if(std::find(operators.begin(), operators.end(), var.getName()) != operators.end())
	{
		logger.error("Operator " + var.getName() + " cannot be used as variable name");
		exit(1);
	}
	else
		variables.back()[var.getName()] = var;
}

void GlobalInfo::addFunction(const Function& function)
{
	if(std::find(reservedWords.begin(), reservedWords.end(), function.getName()) != reservedWords.end())
	{
		logger.error("Reserved word " + function.getName() + " cannot be used as variable name");
		exit(1);
	}
	else if(std::find(operators.begin(), operators.end(), function.getName()) != operators.end())
	{
		logger.error("Operator " + function.getName() + " cannot be used as variable name");
		exit(1);
	}
	else
	{
		for(auto &scope: functions)
		{
			if(scope.find(function.getName()) != scope.end())
			{
				for(auto &func: scope[function.getName()])
				{
					if(function == func)
					{
						logger.error("Function " + function.getName() + " already exists");
						exit(1);
					}
				}
			}
		}

		functions.back()[function.getName()].push_back(function);
	}
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
	functions.emplace_back();
}

void GlobalInfo::decreaseScope()
{
	variables.pop_back();
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

	logger.error("Variable " + name + " not found");
	exit(1);
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
