#include "DFA.h"

namespace Automata {

DFA::DFA(const DFA& dfa)
:_nfa(dfa._nfa)
{
}

DFA::DFA(const NFA& nfa)
:_nfa(nfa)
{
}

StateType DFA::getInitialState() const
{
	return _nfa.getInitialState();
}

StateSetType DFA::getFinalStates() const
{
	return _nfa.getFinalStates();
}

StateType DFA::move(const StateType& from, const SymbolType& symbol) const
{
	auto targets = _nfa.move(from, symbol);
	if(targets.empty())
		throw std::invalid_argument("No transition from " + std::to_string(from) + " with symbol " + symbol);
	return *_nfa.move(from, symbol).begin();
}

TransitionTable::TransitionIteratorTag DFA::getTransitions(const StateType& state) const
{
	return _nfa.getTransitions(state);
}

DFARunner::DFARunner(const DFA& dfa)
:_runner(dfa._nfa)
{
}

std::ostream& operator<<(std::ostream& os, const DFA& dfa)
{
	os << dfa._nfa;
	return os;
}

TransitionTable::StateIterator begin(const DFA& dfa)
{
	return begin(dfa._nfa);
}

TransitionTable::StateIterator end(const DFA& dfa)
{
	return end(dfa._nfa);
}

bool DFARunner::run(const std::string& input)
{
	return _runner.run(input);
}

} /* namespace Automata */
