#pragma once

#include "pch.hpp"

class Variable
{
private:
	std::string name;
	std::string type;
	std::string value;

	static std::string add(std::string a, std::string b);

public:
	Variable(std::string &line, GlobalInfo &globals);

	Variable(std::vector<std::string> lineInfo, GlobalInfo &globals);

	[[nodiscard]] std::string getType() const
	{ return type; }

	[[nodiscard]] std::string getStringValue() const
	{ return value; }

	[[nodiscard]] long getIntValue() const
	{ return std::stol(value); }

	[[nodiscard]] unsigned long getUIntValue() const
	{ return std::stoul(value); }

	[[nodiscard]] double getFloatValue() const
	{ return std::stod(value); }

	[[nodiscard]] bool getBoolValue() const;

//	ClassDef getClassValue()
//	{ return ClassDef(value); }

	[[nodiscard]] std::string getName() const
	{ return name; }

	static std::string evaluate(std::string line, GlobalInfo &globals);
};