#ifndef DFA_H_
#define DFA_H_

#include <algorithm>
#include <iostream>

#include "Common.h"
#include "TransitionTable.h"
#include "NFA.h"

namespace Automata {

class DFARunner;

class DFA
{
protected:
	const NFA _nfa;

public:
	DFA(const DFA&);
	DFA(const NFA&);
	StateType getInitialState() const;
	StateSetType getFinalStates() const;
	StateType move(const StateType&, const SymbolType&) const;
	TransitionTable::TransitionIteratorTag getTransitions(const StateType& state) const;
	// Friend classes
	friend class DFARunner;
	// Friend methods
	friend std::ostream& operator<<(std::ostream&, const DFA&);
	friend TransitionTable::StateIterator begin(const DFA&);
	friend TransitionTable::StateIterator end(const DFA&);
	friend TransitionTable::TransitionIterator begin(const TransitionTable::TransitionIteratorTag&);
	friend TransitionTable::TransitionIterator end(const TransitionTable::TransitionIteratorTag&);
};

template <class LabelType>
class DFABuilder
{
protected:
	using TransitionType = std::tuple<LabelType, SymbolType, LabelType>;

	std::vector<TransitionType> _transitions;
	NFABuilder<LabelType> _nfaBuilder;

public:
	DFABuilder() = default;
	DFABuilder(const DFABuilder&) = delete;
	DFABuilder& operator=(const DFABuilder&) = delete;
	void setInitialStateLabel(const LabelType& initialStateLabel){ _nfaBuilder.setInitialStateLabel(initialStateLabel); }
	void addFinalStateLabel(const LabelType& finalStateLabel){ _nfaBuilder.addFinalStateLabel(finalStateLabel); }
	void addTransition(const LabelType& startLabel, const SymbolType& symbol, const LabelType& finalLabel)
	{
		if(symbol == Epsilon)
			throw std::invalid_argument("Epsilon can not be used as a transition symbol.");
		for(const auto& t: _transitions)
			if(std::get<0>(t) == startLabel && std::get<1>(t) == symbol && std::get<2>(t) != finalLabel)
				throw std::invalid_argument("Same source and same symbol can not go to different targets.");

		const auto transition = std::make_tuple(startLabel, symbol, finalLabel);
		if(std::find(std::begin(_transitions), std::end(_transitions), transition) == std::end(_transitions))
			_transitions.push_back(transition);
	}
	DFA build()
	{
		for(const auto& t: _transitions)
			_nfaBuilder.addTransition(std::get<0>(t), std::get<1>(t), std::get<2>(t));
		return DFA(_nfaBuilder.build());
	}
};

class DFARunner
{
protected:
	NFARunner _runner;

public:
	DFARunner(const DFA&);
	DFARunner(const DFARunner&) = delete;
	DFARunner& operator=(const DFARunner&) = delete;
	bool run(const std::string&);
};

} /* namespace Automata */

#endif /* DFA_H_ */
