#include <fstream>
#include <iostream>
#include "Variable.hpp"
#include "helperFunctions.hpp"

bool Logger::debug = true;

int main(int argc, char** args)
{
	Logger mainLogger("main");
	if(argc != 2)
		std::cout << "Usage: " << args[0] << " input.txt";

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

	mainLogger.log("Parsing file: %s", args[1]);
	for(const auto& line : lineSplit(fileText))
		Variable::evaluate(line);

	return 0;
}
