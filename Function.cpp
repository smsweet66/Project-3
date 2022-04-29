#include "Function.hpp"

bool Function::matches(const std::string &name, const std::vector<std::string> parameterTypes) const
{
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
	std::stringstream ss(parameterInfo);
	std::string token;
	while (std::getline(ss, token, ','))
	{
		std::stringstream ss2(token);
		std::string type;
		std::string varName;
		std::getline(ss2, varName, ':');
		std::getline(ss2, type, ' ');
		this->parameterInfo.emplace_back(varName, type);
	}
}

// Function has already been matched, so check is not required
std::string Function::run(const std::vector<Variable> &parameterValues) const
{
	GlobalInfo::increaseScope();
	for(auto &parameter : parameterValues)
		GlobalInfo::addVariable(parameter);

	for(auto &statement : lineSplit(body))
	{
		if(statement.empty())
			continue;
		else
		{
			std::string output = Variable::evaluate(statement, GlobalInfo::getScope());
			std::vector<std::string> result = split(output);
			if(result.size() == 2 && result[0] == "return")
			{
				GlobalInfo::decreaseScope();
				return result[1];
			}
			else if(result.size() == 1 && result[0] == "continue")
			{
				GlobalInfo::decreaseScope();
				return "";
			}
		}
	}

	GlobalInfo::decreaseScope();
	return "";
}
