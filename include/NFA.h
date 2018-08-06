#ifndef NFA_H_
#define NFA_H_

#include <queue>
#include <iostream>

#include "TransitionTable.h"
#include "Common.h"

namespace Automata {

class EpsilonClosure;
template <class LabelType> class NFABuilder;

class NFA
{
private:
	const StateType _initialState;
	const StateSetType _finalStates;
	const TransitionTable _transitions;

public:
	NFA(const NFA&);
	NFA(const TransitionTable&, const StateType&, const StateSetType&);
	StateType getInitialState() const;
	const StateSetType& getFinalStates() const;
	StateSetType getFinalStates();
	const StateSetType& move(const StateType&, const SymbolType&) const;
	StateSetType move(const StateType&, const SymbolType&);
	TransitionTable::TransitionIteratorTag getTransitions(const StateType&) const;
	~NFA() = default;
	// Friend classes
	friend class EpsilonClosure;
	template <class LabelType> friend class NFABuilder;
	// Friend methods
	friend std::ostream& operator<<(std::ostream&, const NFA&);
	friend TransitionTable::StateIterator begin(const NFA&);
	friend TransitionTable::StateIterator end(const NFA&);
	friend TransitionTable::TransitionIterator begin(const TransitionTable::TransitionIteratorTag&);
	friend TransitionTable::TransitionIterator end(const TransitionTable::TransitionIteratorTag&);
};

template <class LabelType>
class NFABuilder
{
protected:
	using TransitionType = std::tuple<LabelType, SymbolType, LabelType>;

	std::vector<TransitionType> _transitions;
	LabelType _initialStateLabel;
	bool _initialStateLabelSet;
	typename std::set<LabelType> _finalStatesLabels;

public:
	NFABuilder():_transitions(), _initialStateLabel(), _initialStateLabelSet(false), _finalStatesLabels() {}
	NFABuilder(const NFABuilder&) = delete;
	NFABuilder& operator=(const NFABuilder&) = delete;
	void setInitialStateLabel(const LabelType& initialStateLabel)
	{
		_initialStateLabel = initialStateLabel;
		_initialStateLabelSet = true;

	}
	void addFinalStateLabel(const LabelType& finalStateLabel)
	{
		_finalStatesLabels.insert(finalStateLabel);
	}
	void addTransition(const LabelType& startLabel, const SymbolType& symbol, const LabelType& endLabel)
	{
		_transitions.push_back(std::make_tuple(startLabel, symbol, endLabel));
	}
	NFA build()
	{
		if(_initialStateLabelSet == false)
			throw std::invalid_argument("Initial state label was not defined");

		const auto alphabet = buildAlphabet();

		TransitionTable transitionTable;
		const auto labelStateMapping = buildMapping(transitionTable);

		for(const auto& symbol: alphabet)
			transitionTable.addSymbol(symbol);

		for(const auto& t: _transitions)
		{
			const auto startLabel = std::get<0>(t);
			const auto startState = labelStateMapping.find(startLabel)->second;
			const auto endLabel = std::get<2>(t);
			const auto endState = labelStateMapping.find(endLabel)->second;
			const auto symbol = std::get<1>(t);

			transitionTable.addTransition(startState, symbol, endState);
		}

		StateSetType finalStates;
		for(const auto& finalStateLabel: _finalStatesLabels)
		{
			const auto iter = labelStateMapping.find(finalStateLabel);
			if(iter != std::end(labelStateMapping))
				finalStates.insert(iter->second);
		}

		StateType initialState = labelStateMapping.find(_initialStateLabel)->second;

		return NFA(transitionTable, initialState, finalStates);
	}
	~NFABuilder() = default;

protected:
	AlphabetType buildAlphabet() const
	{
		AlphabetType alphabet;
		for(const auto& t: _transitions)
		{
			const SymbolType symbol = std::get<1>(t);
			if(symbol != Epsilon)
				alphabet.insert(symbol);
		}
		return alphabet;
	}
	std::map<LabelType, StateType> buildMapping(TransitionTable& transitionTable) const
				{
		std::map<LabelType, StateType> mapping;
		mapping[_initialStateLabel] = transitionTable.addState();

		for(const auto& t: _transitions)
		{
			const auto startLabel = std::get<0>(t);
			if(mapping.find(startLabel) == std::end(mapping))
				mapping[startLabel] = transitionTable.addState();

			const auto endLabel = std::get<2>(t);
			if(mapping.find(endLabel) == std::end(mapping))
				mapping[endLabel] = transitionTable.addState();
		}

		return mapping;
				}
};

class EpsilonClosure
{
private:
	using MappingType = std::map<StateType, StateSetType>;

	MappingType _mapping;

public:
	EpsilonClosure(const NFA&);
	const StateSetType& getClosure(const StateType&) const;
	StateSetType getClosure(const StateType&);
	StateSetType getClosure(const StateSetType&) const;

	static StateSetType calculateClosure(const NFA&, const StateType&);
};

class NFARunner
{
private:
	const NFA _nfa;

public:
	NFARunner(const NFA&);
	NFARunner(const NFARunner&) = delete;
	NFARunner& operator=(NFARunner) = delete;
	bool run(const std::string&);

private:
	bool finalStateReached(const StateSetType&, const StateSetType&) const;
};

} /* namespace Automata */

#endif /* NFA_H_ */
