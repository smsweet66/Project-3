#include "pch.hpp"

void GlobalInfo::addVariable(std::string line, bool constant)
{
	std::vector<std::string> substrings = split(line);
	//TODO add code to evaluate the right side of the assignment
}

void GlobalInfo::addClassDef(const std::string& line)
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

void GlobalInfo::addFunction(const std::string& line)
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