#include "Variable.hpp"

Variable::Variable(std::string &line)
{
	//TODO
}

Variable::Variable(const std::vector<std::string> &lineInfo)
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

std::string Variable::evaluate(std::string expression)
{
	if(expression[0] == '(' && expression[expression.size() - 1] == ')')
		expression = expression.substr(1, expression.length() - 2);

	auto substrings = split(expression);
	if(substrings[0] == "var")
		GlobalInfo::addVariable(Variable(std::vector<std::string>(substrings.begin() + 1, substrings.end())));
	else if(substrings[0] == "if")
	{
		if(evaluate(substrings[1]) == "true")
			return substrings[2];
		else if(evaluate(substrings[1]) == "false")
			return substrings[3];
		else
		{
			std::cout << "boolean value is wrong, expected true or false, got " << substrings[1] << " instead" << std::endl;
			exit(1);
		}
	}
	else if(substrings[0] == "def")
		GlobalInfo::addFunction(Function(substrings[1], substrings[2], substrings[4], substrings[3]));
	else if(substrings[0] == "class")
		GlobalInfo::addClassDef(ClassDef(substrings[1], substrings[2]));
	else if(substrings[0][0] == '{')
	{
		GlobalInfo::increaseScope();
		evaluate(substrings[0].substr(1, substrings[0].length() - 2));
		GlobalInfo::decreaseScope();
	}
	else if(substrings[1] == "=")
	{
		if(GlobalInfo::isVariable(substrings[0]))
		{
			std::string rhs = substrings[2];
			for(int i = 3; i < substrings.size(); i++)
				rhs += " " + substrings[i];

			rhs = evaluate(rhs);
			GlobalInfo::getVariable(substrings[0]).setValue(rhs);
			return rhs;
		}
		else
		{
			std::cout << "variable " << substrings[0] << " is not defined" << std::endl;
			exit(1);
		}
	}
	else    //not defining anything, so it's just a normal expression
	{

	}

}

std::string Variable::add(std::string a, std::string b)
{
	return a + b;
}