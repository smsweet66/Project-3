#include "GlobalInfo.hpp"
#include "helperFunctions.hpp"

#include <iostream>

Logger Variable::logger("Variable");

Variable::Variable(std::string line)
{
	logger.log("Creating anonymous variable from line: " + line);
	if(line.empty())
		this->type = "NONE";
	else if(line[0] == '\"')
	{
		line = line.substr(1, line.length() - 2);
		this->type = "string";
		this->value = line;
	}
	else if(isFloat(line))
	{
		this->type = "num";
		this->value = std::to_string(stod(line));
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
	else
	{
		logger.error("Unknown variable type: " + line);
		exit(1);
	}
}

Variable::Variable(std::string name, const std::string& type, std::string value)
{
	std::string builtIns[] = {"string", "num", "bool"};
	if(std::find(std::begin(builtIns), std::end(builtIns), type) != std::end(builtIns) || GlobalInfo::isClass(type))
	{
		this->name = std::move(name);
		this->type = type;
		this->value = std::move(value);
	}
	else
	{
		logger.error("Unknown variable type: " + type);
		exit(1);
	}
}

std::string Variable::evaluate(std::string expression, int minScope)
{
	logger.log("Evaluating: " + expression);
	if(expression[0] == '(' && expression[expression.size() - 1] == ')')
		expression = expression.substr(1, expression.length() - 2);

	auto substrings = split(expression);
	if(substrings.size() == 1 && substrings[0] == "return")
		return "return";
	else if(substrings.size() >= 2 && substrings[0] == "return")
	{
		logger.log("Returning: " + expression);
		std::string rhs = substrings[1];
		for(int i = 2; i < substrings.size(); i++)
			rhs += " " + substrings[i];

		return "return " + evaluate(rhs, minScope);
	}
	else if(substrings.size() == 1 && substrings[0][0] != '{')
	{
		if(GlobalInfo::isVariable(substrings[0], minScope))
			return GlobalInfo::getVariable(substrings[0], minScope).value;
		else if(substrings[0][0] == '(')
			return evaluate(substrings[0], minScope);
		else
			return substrings[0];
	}
	else if(substrings[0] == "var")
	{
		std::string rhs = substrings[5];
		for(int i = 6; i < substrings.size(); i++)
			rhs += " " + substrings[i];

		std::string value = evaluate(rhs, minScope);
		GlobalInfo::addVariable(Variable(substrings[1], substrings[3], value));

		return value;
	}
	else if(substrings[0] == "if")
	{
		std::string result = evaluate(substrings[1], minScope);
		if(result == "true")
			return evaluate(substrings[2], minScope);
		else if(result == "false" && substrings.size() == 5)
			return evaluate(substrings[4], minScope);
		else if(result == "false" && substrings.size() == 3)
			return "";
		else
		{
			logger.error("Unknown if statement: " + substrings[1]);
			exit(1);
		}
	}
	else if(substrings.size() >= 2 && substrings[0] == "print")
	{
		std::string rhs = substrings[1];
		for(int i = 2; i < substrings.size(); i++)
			rhs += " " + substrings[i];

		std::cout << evaluate(rhs, minScope) << std::endl;
		return "";
	}
	else if(substrings.size() == 5 && substrings[0] == "def")
	{
		GlobalInfo::addFunction(Function(substrings[1], substrings[2], substrings[4], substrings[3]));
		return "";
	}
	else if(substrings.size() == 3 && substrings[0] == "class")
	{
		GlobalInfo::addClassDef(ClassDef(substrings[1], substrings[2]));
		return "";
	}
	else if(substrings[0][0] == '{')
	{
		GlobalInfo::increaseScope();
		auto lines = lineSplit(substrings[0].substr(1, substrings[0].length() - 2));
		for(int i=0; i<lines.size(); i++)
		{
			std::string result;
			if(lines[i].substr(0, 2) == "if" && i + 1 < lines.size() && lines[i+1].substr(0,4) == "else")
			{
				result = evaluate(lines[i] + " " + lines[i + 1], minScope);
				i++;
			}
			else
				result = evaluate(lines[i], minScope);

			auto resultSubstrings = split(result);
			if(resultSubstrings.size() == 1 && resultSubstrings[0] == "return")
			{
				logger.log("Returning from scope: " + std::to_string(GlobalInfo::getScope()));
				GlobalInfo::decreaseScope();
				return "return";
			}
			else if(resultSubstrings.size() == 2 && resultSubstrings[0] == "return")
			{
				logger.log("Returning from scope: " + std::to_string(GlobalInfo::getScope()));
				GlobalInfo::decreaseScope();
				return "return " + resultSubstrings[1];
			}
		}

		GlobalInfo::decreaseScope();
		return "";
	}
	else if(substrings.size() > 2 && substrings[1] == "=")
	{
		if(GlobalInfo::isVariable(substrings[0], minScope))
		{
			std::string rhs = substrings[2];
			for(int i = 3; i < substrings.size(); i++)
				rhs += " " + substrings[i];

			rhs = evaluate(rhs, minScope);
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

					rhsValue = evaluate(rhsValue, minScope);
					GlobalInfo::getVariable(lhs, minScope).setClassVariable(rhs, rhsValue);
					return rhsValue;
				}
				else
				{
					logger.error("Class member not found: " + rhs);
					exit(1);
				}
			}
			else
			{
				logger.error("Class instance not found: " + lhs);
				exit(1);
			}
		}
		else
		{
			logger.error("Variable not found: " + substrings[0]);
			exit(1);
		}
	}
	else    //not defining anything, so it's just a normal expression
	{
		enum class States
		{
			functionCall,   // handles function calls and variable access
			multiplication, // handles multiplication and division
			addition,       // handles addition and subtraction
			boolean        // handles boolean operators
		};

		std::array statesList = { States::functionCall, States::multiplication, States::addition, States::boolean };

		for(auto state : statesList)
		{
			for(int i = 0; i < substrings.size(); i++)
			{
				switch(state)
				{
				case States::functionCall:
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
					else if(GlobalInfo::isFunction(substrings[i], minScope))
					{
						logger.log("function found: " + substrings[i]);
						std::string parametersRaw = substrings[i + 1];
						std::vector<std::string> parameterStringsRaw = split(parametersRaw.substr(1, parametersRaw.size() - 2));
						std::vector<std::string> parameterStrings;
						std::string buffer;
						int paramNum = 0;
						for(auto &param: parameterStringsRaw)
						{
							if(param == ",")
							{
								parameterStrings.push_back(buffer);
								buffer = "";
								paramNum++;
							}
							else
								buffer += " " + param;
						}

						parameterStrings.push_back(buffer);
						std::vector<Variable> parameters;
						for(auto &param: parameterStrings)
						{
							logger.log("parameter: " + param);
							parameters.emplace_back(Variable(evaluate(param, minScope)));
						}

						std::vector<std::string> parameterTypes;
						for(auto &param: parameters)
						{
							logger.log("parameter type: " + param.getType());
							parameterTypes.emplace_back(param.getType());
						}

						logger.log("running function: %s with %d parameters", substrings[i].c_str(), parameterTypes.size());
						substrings[i] = GlobalInfo::getFunction(substrings[i], parameterTypes, minScope).run(parameters);
						substrings.erase(substrings.begin() + i + 1);
					}
					break;

				case States::multiplication:
					if(substrings[i] == "*")
					{
						std::string left = substrings[i - 1];
						std::string right = substrings[i + 1];

						left = evaluate(left, minScope);
						right = evaluate(right, minScope);
						logger.log("found * operator, left: %s, right: %s", left.c_str(), right.c_str());

						substrings[i] = std::to_string(std::stod(left) * std::stod(right));
						logger.log("result: %s", substrings[i].c_str());
						substrings.erase(substrings.begin() + i + 1);
						substrings.erase(substrings.begin() + i - 1);
					}
					else if(substrings[i] == "/")
					{
						std::string left = substrings[i - 1];
						std::string right = substrings[i + 1];

						left = Variable(evaluate(left, minScope)).toString();
						right = Variable(evaluate(right, minScope)).toString();
						logger.log("found / operator, left: %s, right: %s", left.c_str(), right.c_str());

						substrings[i] = std::to_string(std::stod(left) / std::stod(right));
						logger.log("result: %s", substrings[i].c_str());
						substrings.erase(substrings.begin() + i + 1);
						substrings.erase(substrings.begin() + i - 1);
					}

					break;
				case States::addition:
					if(substrings[i] == "+")
					{
						std::string left = substrings[i - 1];
						std::string right = substrings[i + 1];

						left = Variable(evaluate(left, minScope)).toString();
						right = Variable(evaluate(right, minScope)).toString();
						logger.log("found + operator, left %s, right %s", left.c_str(), right.c_str());

						substrings[i] = std::to_string(std::stod(left) + std::stod(right));
						logger.log("result is %s", substrings[i].c_str());
						substrings.erase(substrings.begin() + i + 1);
						substrings.erase(substrings.begin() + i - 1);
					}
					else if(substrings[i] == "-")
					{
						std::string left = substrings[i - 1];
						std::string right = substrings[i + 1];

						left = Variable(evaluate(left, minScope)).toString();
						right = Variable(evaluate(right, minScope)).toString();
						logger.log("found - operator, left: %s, right: %s", left.c_str(), right.c_str());

						substrings[i] = std::to_string(std::stod(left) - std::stod(right));
						logger.log("result: %s", substrings[i].c_str());
						substrings.erase(substrings.begin() + i + 1);
						substrings.erase(substrings.begin() + i - 1);
					}

					break;
				case States::boolean:
					if(substrings[i] == "!")
					{
						std::string right = substrings[i + 1];
						right = evaluate(right, minScope);

						logger.log("found ! operator, right: %s", right.c_str());
						if(right == "true")
						{
							substrings[i + 1] = "false";
							substrings.erase(substrings.begin() + i);
						}
						else if(right == "false")
						{
							substrings[i + 1] = "true";
							substrings.erase(substrings.begin() + i);
						}
						else
						{
							std::cout << "boolean operator ! can only be used on a boolean" << std::endl;
							exit(1);
						}
					}
					else if(substrings[i] == "!=")
					{
						std::string left = substrings[i - 1];
						std::string right = substrings[i + 1];

						left = Variable(evaluate(left, minScope)).toString();
						right = Variable(evaluate(right, minScope)).toString();

						logger.log("found != operator, left: %s, right: %s", left.c_str(), right.c_str());
						if(left == right)
							substrings[i] = "false";
						else
							substrings[i] = "true";

						logger.log("result: %s", substrings[i].c_str());
						substrings.erase(substrings.begin() + i + 1);
						substrings.erase(substrings.begin() + i - 1);
					}
					else if(substrings[i] == "==")
					{
						std::string left = substrings[i - 1];
						std::string right = substrings[i + 1];

						left = Variable(evaluate(left, minScope)).toString();
						right = Variable(evaluate(right, minScope)).toString();

						logger.log("found == operator, left: %s, right: %s", left.c_str(), right.c_str());
						if(left == right)
							substrings[i] = "true";
						else
							substrings[i] = "false";

						logger.log("result: %s", substrings[i].c_str());
						substrings.erase(substrings.begin() + i + 1);
						substrings.erase(substrings.begin() + i - 1);
					}
					else if(substrings[i] == "&&")
					{
						std::string left = substrings[i - 1];
						std::string right = substrings[i + 1];

						left = evaluate(left, minScope);
						right = evaluate(right, minScope);

						logger.log("found && operator, left: %s, right: %s", left.c_str(), right.c_str());
						if(left == "true" && right == "true")
							substrings[i] = "true";
						else
							substrings[i] = "false";

						logger.log("result: %s", substrings[i].c_str());
						substrings.erase(substrings.begin() + i + 1);
						substrings.erase(substrings.begin() + i - 1);
					}
					else if(substrings[i] == "||")
					{
						std::string left = substrings[i - 1];
						std::string right = substrings[i + 1];

						left = evaluate(left, minScope);
						right = evaluate(right, minScope);

						logger.log("found || operator, left: %s, right: %s", left.c_str(), right.c_str());
						if(left == "true" || right == "true")
							substrings[i] = "true";
						else
							substrings[i] = "false";

						logger.log("result: %s", substrings[i].c_str());
						substrings.erase(substrings.begin() + i + 1);
						substrings.erase(substrings.begin() + i - 1);
					}

					break;

				default:
					break;
				}
			}
		}

		return substrings[0];
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