#include "pch.hpp"

int main(int argc, char** args)
{
	if(argc != 2)
		std::cout << "Usage: " << args[0] << " input.c!";

	std::fstream input(args[1]);
	GlobalInfo globals;

	return 0;
}
