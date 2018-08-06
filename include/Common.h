#ifndef COMMON_H_
#define COMMON_H_

#include <set>
#include <string>
#include <iostream>
#include <sstream>

namespace Automata
{
	using StateType = unsigned int;
	using StateSetType = std::set<StateType>;
	using SymbolType = std::string;
	using SymbolSetType = std::set<SymbolType>;
	using AlphabetType = SymbolSetType;

	const SymbolType Epsilon = "#";

	template <class T>
	std::ostream& operator<<(std::ostream& os, const std::set<T>& s)
	{
		os << "{";
		if(!s.empty())
		{
			auto iter = std::begin(s);
			os << *iter;
			++iter;
			while(iter != std::end(s))
			{
				os << ", " << *iter;
				++iter;
			}
		}
		return os << "}";
	}

	template <class T>
	std::string to_string(const T& x)
	{
		std::ostringstream oss;
		oss << x;
		return oss.str();
	}
}


#endif /* COMMON_H_ */
