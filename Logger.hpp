#pragma once

#include <string>

class Logger
{
public:
	explicit Logger(const char* logName)
	: name(logName) {}

	void log(std::string msg, ...) const;

	void warn(std::string msg, ...) const;

	void error(std::string msg, ...) const;

private:
	std::string name;
	static bool debug;
};