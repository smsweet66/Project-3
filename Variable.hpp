#include <utility>

#pragma once

enum class types
{
	String,
	Int,
	Uint,
	Float,
	Bool,
};

class Variable
{
private:
	const types type;
	std::string value;
	bool constant;

public:
	Variable(types type, std::string value, bool constant)
	: type(type), value(std::move(value)), constant(constant) {}

	types getType() const
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
};