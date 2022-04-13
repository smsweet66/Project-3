#pragma once

#include <list>

#include "ClassDef.h"

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s)
{ return rtrim(ltrim(s)); }

class Variable
{
private:
	std::string type;
	std::string value;
	bool constant;

public:
	Variable(std::string type, std::string value, bool constant)
	: type(std::move(type)), value(std::move(value)), constant(constant) {}

	[[nodiscard]] std::string getType() const
	{ return type; }

	std::string& getStringValue()
	{ return value; }

	long getIntValue()
	{ return std::stol(value); }

	unsigned long getUIntValue()
	{ return std::stoul(value); }

	double getFloatValue()
	{ return std::stod(value); }

	bool getBoolValue()
	{
		if(value == "true")
			return true;
		else if(value == "false")
			return false;
		else
		{
			std::cout << "boolean value is wrong, expected true or false, got " << value << " instead" << std::endl;
			exit(1);
		}
	}

	ClassDef getClassValue()
	{ return ClassDef(value); }
};

class globalInfo
{
private:
	std::unordered_map<std::string, Variable&> variables;
	std::unordered_map<std::string, ClassDef&> classes;
	std::unordered_map<std::string, std::list<Function>> functions;

public:
	globalInfo() = default;

	void addVariable(std::string line, bool constant)
	{
		std::string name = trim(line.substr(0, line.find(':')));
		line = trim(line.substr(line.find(':') + 1));
		std::string type = trim(line.substr(0, line.find(' ')));
		line = trim(line.substr(line.find(' ') + 1));
		//TODO add code to evaluate the right side of the assignment
	}

	void addClassDef(const std::string& line)
	{
		ClassDef classDef(line);
		if(classes.find(classDef.getName()) != classes.end())
		{
			std::cout << "Class " << classDef.getName() << " already defined" << std::endl;
			exit(1);
		}
		else
			classes[classDef.getName()] = classDef;
	}

	void addFunction(const std::string& line)
	{
		Function function(line);
		if(std::find(functions[function.getName()].begin(), functions[function.getName()].end(), function) != functions[function.getName()].end())
		{
			std::cout << "Function " << function.getName() << " already defined" << std::endl;
			exit(1);
		}
		else
			functions[function.getName()].push_back(function);
	}

};