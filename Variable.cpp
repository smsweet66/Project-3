#include "Variable.hpp"

#include <utility>

Variable::Variable(std::string line)
{
	if(line.empty())
		this->type == "NONE";
	else if(line[0] == '\"')
	{
		line = line.substr(1, line.length() - 2);
		this->type = "string";
		this->value = line;
	}
	else if(isFloat(line))
	{
		this->type = "num";
		this->value = line;
	}
	else if(GlobalInfo::isClass(split(line)[0]))
	{
		this->type = split(line)[0];
		this->value = line;
	}
	else if(line == "false" || line == "true")
	{
		this->type = "bool";
		this->value = line;
	}
	else if(line[0] == '[' && line[line.length() - 1] == ']')
	{
		std::string listContents = line.substr(1, line.length() - 2);
		std::vector<std::string> listContentsVector = split(listContents);
		Variable var(listContentsVector[0]);
		this->type = "[" + var.type + "]";
		this->value = line;
	}
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

std::string Variable::evaluate(std::string expression, int minScope)
{
	if(expression[0] == '(' && expression[expression.size() - 1] == ')')
		expression = expression.substr(1, expression.length() - 2);

	auto substrings = split(expression);
	if(substrings[0] == "var")  //TODO change this section to actually evaluate rhs
		GlobalInfo::addVariable(Variable(std::vector<std::string>(substrings.begin() + 1, substrings.end())));
	else if(substrings[0] == "if")
	{
		std::string result = evaluate(substrings[1], minScope);
		if(result == "true")
			return evaluate(substrings[2], minScope);
		else if(result == "false" && substrings.size() == 4)
			return evaluate(substrings[3], minScope);
		else if(result == "false" && substrings.size() == 3)
			return "";
		else
		{
			std::cout << "boolean value is wrong, expected true or false, got " << substrings[1] << " instead" << std::endl;
			exit(1);
		}
	}
	else if(substrings.size() == 5 && substrings[0] == "def")
		GlobalInfo::addFunction(Function(substrings[1], substrings[2], substrings[4], substrings[3]));
	else if(substrings.size() == 3 && substrings[0] == "class")
		GlobalInfo::addClassDef(ClassDef(substrings[1], substrings[2]));
	else if(substrings[0][0] == '{')
	{
		GlobalInfo::increaseScope();
		evaluate(substrings[0].substr(1, substrings[0].length() - 2), minScope);
		GlobalInfo::decreaseScope();
	}
	else if(substrings.size() > 2 && substrings[1] == "=")
	{
		if(GlobalInfo::isVariable(substrings[0], minScope))
		{
			std::string rhs = substrings[2];
			for(int i = 3; i < substrings.size(); i++)
				rhs += " " + substrings[i];

			rhs = evaluate(rhs);
			GlobalInfo::getVariable(substrings[0], minScope).setValue(rhs);
			return rhs;
		}
		else if(substrings[0].find('.') != std::string::npos)
		{
			std::string lhs = substrings[0].substr(0, substrings[0].find('.'));     //class instance name
			std::string rhs = substrings[0].substr(substrings[0].find('.') + 1);    //class member name
			if(GlobalInfo::isVariable(lhs, minScope))
			{
				if(GlobalInfo::getClassDef(GlobalInfo::getVariable(lhs, minScope).getType()).containsVariable(rhs))
				{
					std::string rhsValue = substrings[2];
					for(int i = 3; i < substrings.size(); i++)
						rhsValue += " " + substrings[i];

					rhsValue = evaluate(rhsValue);
					GlobalInfo::getVariable(lhs, minScope).setClassVariable(rhs, rhsValue);
					return rhsValue;
				}
				else
				{
					std::cout << "class " << GlobalInfo::getVariable(lhs, minScope).getType() << " does not have a member named " << rhs << std::endl;
					exit(1);
				}
			}
			else
			{
				std::cout << "variable " << lhs << " does not exist" << std::endl;
				exit(1);
			}
		}
		else
		{
			std::cout << "variable " << substrings[0] << " is not defined" << std::endl;
			exit(1);
		}
	}
	else    //not defining anything, so it's just a normal expression
	{
		enum class States
		{
			functionCall,   // handles function calls, variable access, and list access
			multiplication, // handles multiplication and division
			addition,       // handles addition and subtraction
			boolean        // handles boolean operators
		};

		std::array statesList = { States::functionCall, States::multiplication, States::addition, States::boolean };

		for(auto state : statesList)
		{
			for(int i = 0; i < substrings.size(); i++)
			{
				if(state == States::functionCall)
				{
					if(substrings[i] == "." && GlobalInfo::isVariable(substrings[i - 1], minScope))
					{
						Variable &instance = GlobalInfo::getVariable(substrings[i - 1], minScope);
						if(GlobalInfo::isClass(instance.getType()))
						{
							ClassDef &classDef = GlobalInfo::getClassDef(instance.getType());
							std::string methodName = substrings[i + 1];
							if(classDef.containsVariable(methodName))
							{
								std::vector<std::string> newSubstrings;
								for(int j=0; j<i-1; j++)
									newSubstrings.push_back(substrings[j]);

								newSubstrings.push_back(instance.getClassVariable(methodName));
								for(int j=i+2; j<substrings.size(); j++)
									newSubstrings.push_back(substrings[j]);

								i++;
							}
							else
							{
								std::string parametersRaw = substrings[i + 2];
								std::vector<std::string> parameters = split(parametersRaw);
								std::vector<std::string> evaluatedParameters;
								int paramNum = 0;
								for(auto &param: parameters)
								{
									if(param == ",")
										paramNum++;
									else
										evaluatedParameters[i] += param;
								}

								for(auto &param: evaluatedParameters)
									param = evaluate(param, minScope);

								std::vector<Variable> params;
								for(auto &param: evaluatedParameters)
									params.emplace_back(Variable(param));

								std::vector<std::string> parameterTypes;
								for(auto &param: params)
									parameterTypes.emplace_back(param.getType());
								if(classDef.containsMethod(methodName, parameterTypes))
								{
									Function &method = classDef.getMethod(methodName, parameterTypes);
									std::string returnValue = method.run(params);

									// replaces the function call with the return value
									std::vector<std::string> newSubstrings;
									for(int j = 0; j < i; j++)
										newSubstrings.emplace_back(substrings[j]);
									newSubstrings.emplace_back(returnValue);
									for(int j = i + 3; j < substrings.size(); j++)
										newSubstrings.emplace_back(substrings[j]);

									substrings = newSubstrings;
								}
								else
								{
									std::cout << "method " << methodName << " is not defined for class "
									          << instance.getType()
									          << std::endl;
									exit(1);
								}
							}
						}
						else
						{
							std::cout << "variable " << substrings[i - 1] << " is not an instance of a class"
							          << std::endl;
							exit(1);
						}
					}
				}
			}
		}
	}
}

std::string Variable::getClassVariable(const std::string& varName)
{
	std::vector<std::string> substrings = split(value);
	int nameIndex = 0;
	while(substrings[nameIndex] != varName)
		nameIndex++;

	return {substrings[nameIndex + 1]};
}

void Variable::setClassVariable(const std::string &varName, std::string newValue)
{
	std::vector<std::string> substrings = split(this->value);
	int nameIndex = 0;
	while(substrings[nameIndex] != varName)
		nameIndex++;

	substrings[nameIndex + 1] = std::move(newValue);

	this->value = "";
	for(auto &substring : substrings)
		this->value += substring + " ";
	this->value.pop_back();
}