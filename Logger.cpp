#include <iostream>
#include <cstdarg>
#include "Logger.hpp"

void Logger::log(std::string msg, ...) const
{
	if(debug)
	{
		std::cout << "\033[32m" << name << " Log: ";

		va_list args;
		va_start(args, msg);
		vfprintf(stdout, msg.c_str(), args);
		std::cout << std::endl;
		va_end(args);

		std::cout << "\033[0m";  //Back to default color
	}
}

void Logger::warn(std::string msg, ...) const
{
	std::cout << "\033[0;33m" << name << "--Warning: ";

	va_list args;
	va_start(args, msg);
	vfprintf(stdout, msg.c_str(), args);
	std::cout << std::endl;
	va_end(args);

	std::cout << "\033[0m";  //Back to default color
}

void Logger::error(std::string msg, ...) const
{
	std::cout << "\033[0;31m" << name << "----Error: ";

	va_list args;
	va_start(args, msg);
	vfprintf(stdout, msg.c_str(), args);
	std::cout << std::endl;
	va_end(args);

	std::cout << "\033[0m";  //Back to default color
}