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
	Variable() = default;

	Variable(const Variable &other) = default;

	Variable(std::string &line);

	Variable(const std::vector<std::string> &lineInfo);

	[[nodiscard]] std::string getType() const
	{ return type; }

	[[nodiscard]] std::string getStringValue() const
	{ return value; }

	[[nodiscard]] double getNumValue() const
	{ return std::stod(value); }

	[[nodiscard]] bool getBoolValue() const;

	void setValue(std::string &val)
	{ value = val; }

//	ClassDef getClassValue()
//	{ return ClassDef(value); }

	[[nodiscard]] std::string getName() const
	{ return name; }

	Variable& operator=(const Variable &other) = default;

	static std::string evaluate(std::string line, int minScope = 0);
};