#include <fstream>
#include <iostream>
#include "Variable.hpp"
#include "helperFunctions.hpp"

bool Logger::debug = false;

int main(int argc, char** args)
{
	Logger mainLogger("main");
	if(argc != 2)
		std::cout << "Usage: " << args[0] << "testFiles/input.c!";

	std::fstream input(args[1]);
	if(!input.is_open())
		std::cout << "Error: Could not open file " << args[1];

	mainLogger.log("Reading file: %s", args[1]);
	std::string fileText;
	std::string buffer;
	while(input >> buffer)
	{
		fileText += buffer;
		fileText += " ";
	}

	input.close();

	fileText = replaceAll(replaceAll(fileText, "{", " { "), "}", " } ");
	fileText = replaceAll(replaceAll(fileText, "+", " + "), "-", " - ");
	fileText = replaceAll(replaceAll(fileText, "*", " * "), "/", " / ");
	fileText = replaceAll(replaceAll(fileText, "=", " = "), ":", " : ");
	fileText = replaceAll(replaceAll(fileText, "!", " ! "), ", ", " , ");
	fileText = replaceAll(replaceAll(fileText, "!  =", "!="), "=  =", "==");
	fileText = replaceAll(replaceAll(fileText, "(", " ( "), ")", " ) ");
	fileText = replaceAll(replaceAll(fileText, "<", " < "), ">", " > ");
	fileText = replaceAll(replaceAll(fileText, "<  =", "<="), ">  =", ">=");
	fileText = replaceAll(replaceAll(fileText, "|", " | "), "|  |", "||");

	mainLogger.log("Parsing file: %s", args[1]);
	auto lines = lineSplit(fileText);
	for(int i=0; i<lines.size(); i++)
	{
		if(lines[i].substr(0, 2) == "if" && i + 1 < lines.size() && lines[i+1].substr(0,4) == "else")
		{
			Variable::evaluate(lines[i] + " " + lines[i + 1]);
			i++;
		}
		else
			Variable::evaluate(lines[i]);

	}

	return 0;
}
