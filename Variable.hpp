#pragma once

#include "ClassDef.h"

class Variable
{
private:
	const std::string type;
	std::string value;
	bool constant;

public:
	const static std::string types[];

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

const std::string Variable::types[] = { "String", "Int", "Uint", "Float", "Bool", "ClassDef", "ClassInstance", "Function" };