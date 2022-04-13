#pragma once

#include <vector>
#include <sstream>
#include <unordered_map>
#include "Variable.hpp"

class Function
{
private:
	std::vector<std::pair<std::string, std::string>> parameterInfo;
	std::string body;

public:
	explicit Function(const std::string& functionInfo)
	{
		std::stringstream ss(functionInfo);
		std::string line;

		std::getline(ss, line);

		std::stringstream ss2(line);
		std::string parameter;

		while (std::getline(ss2, parameter, ','))
		{
			std::stringstream ss3(parameter);
			std::string type;
			std::string name;

			std::getline(ss3, type, ' ');
			std::getline(ss3, name, ' ');

			parameterInfo.emplace_back(type, name);
		}

		while(std::getline(ss, line))
			body += line + "\n";
	}

	[[nodiscard]] std::string toString() const
	{
		std::stringstream ss;
		for(int i=0; i<parameterInfo.size(); i++)
		{
			ss << parameterInfo[i].first << " " << parameterInfo[i].second;
			if(i != parameterInfo.size() - 1)
				ss << ", ";
		}

		ss << "\n" << body;

		return ss.str();
	}
};

class ClassDef
{
private:
	std::vector<std::pair<std::string, std::string>> variables;
	std::vector<std::pair<std::string, std::string>> methods;

public:
	ClassDef(std::vector<std::pair<std::string, std::string>> variables, std::vector<std::pair<std::string, std::string>> methods)
			: variables(std::move(variables)), methods(std::move(methods)) {}

	explicit ClassDef(const std::string& input)
	{
		std::stringstream ss(input);
		std::string buffer;
		while(std::getline(ss, buffer))
		{
			std::string temp;
			if(buffer.find("variable") != std::string::npos)
			{
				std::string type, name;
				std::stringstream(buffer) >> temp >> type >> name;
				variables.emplace_back(type, name);
			}
			else if(buffer.find("method") != std::string::npos)
			{
				std::string type, body;
				std::stringstream(buffer) >> temp >> type >> body;
				methods.emplace_back(type, body);
			}
		}
	}

	[[nodiscard]] std::vector<std::pair<std::string, std::string>>& getVariables()
	{ return variables; }

	[[nodiscard]] std::vector<std::pair<std::string, std::string>>& getMethods()
	{ return methods; }

	[[nodiscard]] std::string toString() const
	{
		std::string result;
		for(auto& variable : variables)
			result += "variable " + variable.first + " " + variable.second + "\n";

		for(auto& method : methods)
			result += "method " + method.first + " " + method.second + "\n";

		return result;
	}
};

class ClassInstance
{
private:
	std::unordered_map <std::string, Variable> variables;

};