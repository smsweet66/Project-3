#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Variable.hpp"

std::unordered_map<std::string, Variable> globals;

int main(int argc, char** args)
{
	if(argc != 2)
		std::cout << "Usage: " << args[0] << " input.c!";

	std::fstream input(args[1]);

	return 0;
}
