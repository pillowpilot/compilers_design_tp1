#ifndef THOMPSON_H_
#define THOMPSON_H_

#include <string>
#include <stack>

#include <Common.h>
#include <NFA.h>

namespace Automata {

class Trivial
{
public:
	Trivial() = delete;
	static Automata::NFA apply(const SymbolType&);
};

class Concatenation
{
public:
	Concatenation() = delete;
	static Automata::NFA apply(const NFA&, const NFA&);
};

class Alternative
{
public:
	Alternative() = delete;
	static Automata::NFA apply(const NFA&, const NFA&);
};

class Kleene
{
public:
	Kleene() = delete;
	static Automata::NFA apply(const NFA&);
};

void copyTransitions(NFABuilder<std::string>&, const NFA&, const std::string& = "");

class Thompson {
public:
	Thompson() = delete;
	static Automata::NFA apply(const std::string&);
	static bool isConcatenationOperator(const char& c){return c == '.';}
	static bool isAlternativeOperator(const char& c){return c == '|';}
	static bool isKleeneOperator(const char& c){return c == '*';}
};

} /* namespace Automata */

#endif /* THOMPSON_H_ */
