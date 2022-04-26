#pragma once

/**
 * Splits a string at spaces into a vector of strings.
 * Keeps sections surrounded by quotes, parenthesis, and square brackets together.
 * @param str
 * @return a vector of the substrings
 */
std::vector<std::string> split(const std::string& str)
{
	std::vector<std::string> result;
	int start = 0;
	for(int i = 0; i < str.size(); ++i)
	{
		if(str[i] == ' ')
			continue;
		else if(str[i] == '\"')
		{
			start = i;
			i++;
			while(str[i] != '\"')
				i++;

			result.push_back(str.substr(start, i + 1 - start));
		}
		else if(str[i] == '(')
		{
			start = i;
			int depth = 1;
			i++;
			while(depth > 0)
			{
				if(str[i] == '(')
					depth++;
				else if(str[i] == ')')
					depth--;
				i++;
			}

			i--;
			result.push_back(str.substr(start, i + 1 - start));
		}
		else if(str[i] == '[')
		{
			start = i;
			int depth = 1;
			i++;
			while(depth > 0)
			{
				if(str[i] == '[')
					depth++;
				else if(str[i] == ']')
					depth--;
				i++;
			}
			i--;
			result.push_back(str.substr(start, i + 1 - start));
		}
		else
		{
			start = i;
			while(i < str.size() && str[i] != ' ')
				i++;

			result.push_back(str.substr(start, i - start));
		}
	}

	return result;
}