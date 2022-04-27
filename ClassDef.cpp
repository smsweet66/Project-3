#include "ClassDef.hpp"

#include <utility>

ClassDef::ClassDef(std::string name, const std::string &body)
: name(std::move(name))
{

}

bool ClassDef::containsMethod(const std::string &funcName, const std::vector<std::string> &parameters) const
{
	return std::any_of(methods.begin(), methods.end(),
					   [&](const Function &func)
					   { return func.matches(funcName, parameters); });
}
