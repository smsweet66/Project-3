#include "Variable.hpp"

Variable::Variable(std::string &line, GlobalInfo &globals)
{
	//TODO
}

Variable::Variable(std::vector<std::string> lineInfo, GlobalInfo &globals)
{
	//TODO
}

bool Variable::getBoolValue() const
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

std::string Variable::evaluate(std::string expression, GlobalInfo &globals)
{
	if(expression[0] == '(')
		expression = expression.substr(1, expression.length() - 2);

	auto substrings = split(expression);
	if(substrings[0] == "var")
		globals.addVariable(Variable(std::vector<std::string>({substrings.begin() + 1, substrings.end()}), globals));
	else if(substrings[0] == "if")
	{
		if(evaluate(substrings[1], globals) == "true")
			return substrings[2];
		else if(evaluate(substrings[1], globals) == "false")
			return substrings[3];
		else
		{
			std::cout << "boolean value is wrong, expected true or false, got " << substrings[1] << " instead" << std::endl;
			exit(1);
		}
	}
	else if(substrings[0] == "def")
		globals.addFunction(Function(substrings[1], substrings[2], substrings[4], substrings[3]));
	else if(substrings[0] == "class")
		globals.addClassDef(ClassDef(substrings[1], substrings[2]));
	else if(substrings[0][0] == '{')
	{
		globals.increaseScope();
		evaluate(substrings[0].substr(1, substrings[0].length() - 2), globals);
		globals.decreaseScope();
	}
	else    //not defining anything, so it's just a normal expression
	{

	}

}

std::string Variable::add(std::string a, std::string b)
{
	return a + b;
}