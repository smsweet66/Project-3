#include "ClassDef.hpp"

#include <utility>

ClassDef::ClassDef(std::string name, const std::string &body)
: name(std::move(name))
{
	auto bodyLines = lineSplit(body);
	for(auto &line : bodyLines)
	{
		auto substrings = split(line);
		if(substrings.empty())
			continue;
		if(substrings[1] == ":")
			variables.emplace_back(substrings[0], substrings[2]);
		else if(substrings[0] == "def")
			methods.emplace_back(Function(substrings[1], substrings[2], substrings[4], substrings[3]));
		else
			throw std::runtime_error("Unknown line in class definition: " + line);
	}
}

bool ClassDef::containsMethod(const std::string &funcName, const std::vector<std::string> &parameters) const
{
	return std::any_of(methods.begin(), methods.end(),
					   [&](const Function &func)
					   { return func.matches(funcName, parameters); });
}
