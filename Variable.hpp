#pragma once

#include "pch.hpp"
#include "GlobalInfo.hpp"

std::string evaluate(std::string expression)
{
	if(expression[0] == '(')
		expression = expression.substr(1, expression.length() - 2);

	auto substrings = split(expression);
	for(int i=0; i<substrings.size(); i++)
	{

	}

}

std::string add(std::string a, std::string b)
{
	return a + b;
}

class Variable
{
private:
	std::string type;
	std::string value;
	bool constant;

public:
	Variable(std::string type, std::string value, bool constant)
	: type(std::move(type)), value(std::move(value)), constant(constant) {}

	Variable(std::string input, GlobalInfo &globals);

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

	bool getBoolValue();

	ClassDef getClassValue()
	{ return ClassDef(value); }
};