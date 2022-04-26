#include "pch.hpp"

Variable::Variable(std::string input, GlobalInfo &globals)
{
	if(input[0] == '(')
	input = evaluate(input);

	if(globals.containsVariable(input))
	{

	}
}

bool Variable::getBoolValue()
{
	if(value == "true")
		return true;
	else if(value == "false")
		return false;
	else
	{
		std::cout << "boolean value is wrong, expected true or false, got " << value << " instead" << std::endl;
		exit(1);
	}
}