#pragma once

#include <string>
#include <utility>
#include <iostream>
#include "Logger.hpp"

class Variable
{
private:
	std::string name;
	std::string type;
	std::string value;
	static Logger logger;

public:
	Variable() = default;

	Variable(const Variable &other) = default;

	// Used for creating an anonymous variable
	explicit Variable(std::string line);

	// Used for creating a named variable
	Variable(std::string name, const std::string& type, std::string value);

	[[nodiscard]] std::string getType() const
	{ return type; }

	[[nodiscard]] std::string toString() const
	{ return value; }

	void setValue(std::string &val);

	[[nodiscard]] std::string getName() const
	{ return name; }

	void setName(const std::string &newName)
	{ name = newName; }

	Variable& operator=(const Variable &other) = default;

	static std::string evaluate(std::string line, int minScope = 0);
};