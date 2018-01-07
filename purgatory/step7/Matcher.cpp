#include <iostream>
#include "Matcher.hpp"

Matcher::Matcher(FSA const &fsa) : m_fsa(fsa)
{
}

Matcher::Matcher(std::string const &pattern) :
	m_fsa(ExpressionParser(pattern).getDFA())
{
}

bool Matcher::find(std::string const &str)
{
	if (m_fsa.initial().empty())
	{
		return false;
	}

	std::string state = m_fsa.initial();
	std::string token;

	for (std::size_t pos = 0; pos < str.length(); ++pos)
	{
		State const &s = m_fsa[state];

		if (s.has(str[pos]))
		{
			state = s[str[pos]];
			token += str[pos];
		} 
		else
		{
			state = m_fsa.initial();
			token = "";
		}
		if (m_fsa[state].isFinal())
		{
			m_fsa[state].execCallback(token);
			return true;
		}
	}
	return false;
}

bool Matcher::find(std::string const &str, std::size_t &count)
{
	count = 0;

	if (m_fsa.initial().empty())
	{
		return false;
	}

	std::string state = m_fsa.initial();
	std::string token;
	bool found = false;

	for (std::size_t pos = 0; pos < str.length(); ++pos)
	{
		State const &s = m_fsa[state];

		if (s.has(str[pos]))
		{
			state = s[str[pos]];
			token += str[pos];
		}
		else
		{
			state = m_fsa.initial();
			token = "";
		}
		if (m_fsa[state].isFinal())
		{
			m_fsa[state].execCallback(token);
			++count;
			found = true;
			state = m_fsa.initial();
		}
	}
	return found;
}
