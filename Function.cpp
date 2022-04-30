#include <sstream>
#include <iostream>
#include "Function.hpp"
#include "GlobalInfo.hpp"
#include "helperFunctions.hpp"

Logger Function::logger = Logger("Function");

bool Function::matches(const std::string &name, const std::vector<std::string> parameterTypes) const
{
	logger.log("Looking for a function with name " + name);
	if (name != this->name)
		return false;

	if (parameterTypes.size() != parameterInfo.size())
		return false;

	for (size_t i = 0; i < parameterTypes.size(); i++)
	{
		if (parameterTypes[i] != parameterInfo[i].second)
			return false;
	}

	return true;
}

Function::Function(std::string &name, std::string &parameterInfo, std::string &body, std::string &returnType)
: name(name), body(body.substr(1, body.size() - 2)), returnType(returnType)
{
	logger.log("Generating function " + name);
	auto parameterInfoSplit = split(parameterInfo.substr(1, parameterInfo.size() - 2));
	for(int i=0; i<parameterInfoSplit.size(); i++)
	{
		if(parameterInfoSplit[i] != ":")
			continue;
		else
		{
			std::string varName = parameterInfoSplit[i-1];
			std::string type = parameterInfoSplit[i+1];
			logger.log("Adding parameter " + varName + " of type " + type);
			this->parameterInfo.emplace_back(varName, type);
		}
	}
}

// Function has already been matched, so check is not required
std::string Function::run(std::vector<Variable> parameterValues) const
{
	logger.log("Running function " + name);
	for(size_t i=0; i<parameterValues.size(); i++)
		parameterValues[i].setName(parameterInfo[i].first);

	GlobalInfo::increaseScope();
	for(auto &parameter : parameterValues)
		GlobalInfo::addVariable(parameter);

	int scope = GlobalInfo::getScope();

	logger.log("Running function body with scope " + std::to_string(scope));
	auto lines = lineSplit(body);
	for(int i=0; i<lines.size(); i++)
	{
		std::string result;
		if(lines[i].substr(0, 2) == "if" && i + 1 < lines.size() && lines[i+1].substr(0,4) == "else")
		{
			result = Variable::evaluate(lines[i] + " " + lines[i + 1], scope);
			i++;
		}
		else
			result = Variable::evaluate(lines[i], scope);

		auto resultSubstrings = split(result);
		if(resultSubstrings.size() == 2 && resultSubstrings[0] == "return")
		{
			logger.log("Returning from function " + name);
			GlobalInfo::decreaseScope();
			return resultSubstrings[1];
		}
		else if(resultSubstrings.size() == 1 && resultSubstrings[0] == "return")
		{
			GlobalInfo::decreaseScope();
			return "";
		}
	}

	GlobalInfo::decreaseScope();
	return "";
}
