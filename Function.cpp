#include "Function.hpp"

bool Function::matches(const std::string &name, const std::vector<std::string> parameterTypes) const
{
	if (name != this->name)
		return false;

	if (parameterTypes.size() != parameterInfo.size())
		return false;

	for (size_t i = 0; i < parameterTypes.size(); i++)
	{
		if (parameterTypes[i] != parameterInfo[i].second)
			return false;
	}

	return true;
}
