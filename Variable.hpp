#pragma once

#include "pch.hpp"

class Variable
{
private:
	std::string name;
	std::string type;
	std::string value;

public:
	Variable() = default;

	Variable(const Variable &other) = default;

	// Used for creating an anonymous variable
	Variable(std::string line);

	// Used for creating a named variable
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

	std::string getClassVariable(const std::string& varName);

	void setClassVariable(const std::string& varName, std::string newValue);

	[[nodiscard]] std::string getName() const
	{ return name; }

	Variable& operator=(const Variable &other) = default;

	static std::string evaluate(std::string line, int minScope = 0);
};