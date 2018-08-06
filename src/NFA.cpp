#include "NFA.h"

namespace Automata {

NFA::NFA(const NFA& nfa)
:_initialState(nfa.getInitialState()), _finalStates(nfa.getFinalStates()), _transitions(nfa._transitions)
{
}

NFA::NFA(const TransitionTable& transitions, const StateType& initialState, const StateSetType& finalStates)
:_initialState(initialState), _finalStates(finalStates), _transitions(transitions)
{
}

StateType NFA::getInitialState() const
{
	return _initialState;
}

const StateSetType& NFA::getFinalStates() const
{
	return _finalStates;
}

StateSetType NFA::getFinalStates()
{
	return const_cast<StateSetType&>(static_cast<const NFA&>(*this).getFinalStates());
}

const StateSetType& NFA::move(const StateType& startState, const SymbolType& symbol) const
{
	return _transitions.getTransition(startState, symbol);
}

StateSetType NFA::move(const StateType& startState, const SymbolType& symbol)
{
	return _transitions.getTransition(startState, symbol);
}

TransitionTable::TransitionIteratorTag NFA::getTransitions(const StateType& state) const
{
	return this->_transitions.getTransitions(state);
}

// Friend functions

std::ostream& operator<<(std::ostream& os, const NFA& nfa)
{
	os << nfa._transitions << std::endl;
	os << "Initial State: " << nfa.getInitialState() << std::endl;
	os << "Final States: " << nfa.getFinalStates() << std::endl;
	return os;
}

TransitionTable::StateIterator begin(const NFA& nfa)
{
	return begin(nfa._transitions);
}

TransitionTable::StateIterator end(const NFA& nfa)
{
	return end(nfa._transitions);
}


// EpsilonClosure
EpsilonClosure::EpsilonClosure(const NFA& nfa)
{
	for(const auto& state: nfa)
	{
		const StateSetType closure = calculateClosure(nfa, state);
		_mapping[state] = closure;
	}
}

const StateSetType& EpsilonClosure::getClosure(const StateType& state) const
{
	if(_mapping.find(state) == std::end(_mapping))
		throw std::invalid_argument("Invalid state");
	return _mapping.find(state)->second;
}

StateSetType EpsilonClosure::getClosure(const StateType& state)
{
	return const_cast<StateSetType&>(static_cast<const EpsilonClosure&>(*this).getClosure(state));
}

StateSetType EpsilonClosure::getClosure(const StateSetType& states) const
{
	StateSetType closures;
	for(const auto& state: states)
	{
		const auto closure = getClosure(state);
		for(const auto& state: closure)
			closures.insert(state);
	}

	return closures;
}

StateSetType EpsilonClosure::calculateClosure(const NFA& nfa, const StateType& initialState)
{
	StateSetType alreadyProcessed;

	StateSetType closure;
	closure.insert(initialState);

	std::queue<StateType> unprocessedStates({initialState});
	while(!unprocessedStates.empty())
	{
		const StateType state = unprocessedStates.front();
		unprocessedStates.pop();
		alreadyProcessed.insert(state);
		for(const auto& nextState: nfa.move(state, Epsilon))
		{
			if(alreadyProcessed.find(nextState) == std::end(alreadyProcessed))
				{
				closure.insert(nextState);
				unprocessedStates.push(nextState);
				}
		}
	}
	return closure;
}

NFARunner::NFARunner(const NFA& nfa)
:_nfa(nfa)
{
}

bool NFARunner::run(const std::string& input)
{
	const StateSetType& finalStates = _nfa.getFinalStates();
	EpsilonClosure closure(_nfa);

	StateSetType currentStates = closure.getClosure(_nfa.getInitialState());

	for(const char& c: input)
	{
		const std::string symbol(1, c);
		StateSetType nextStates;
		for(const auto& currentState: currentStates)
		{
			try
			{
				const StateSetType transition = _nfa.move(currentState, static_cast<SymbolType>(symbol));
				nextStates.insert(std::begin(transition), std::end(transition));
				for(const auto& nextState: transition)
				{
					const StateSetType nextStateClosure = closure.getClosure(nextState);
					nextStates.insert(std::begin(nextStateClosure), std::end(nextStateClosure));
				}
			}
			catch(std::invalid_argument& e)
			{
				return false;
			}
		}
		currentStates = nextStates;
	}

	return finalStateReached(currentStates, finalStates);
}

bool NFARunner::finalStateReached(const StateSetType& currentStates, const StateSetType& finalStates) const
{
	for(const auto& finalState: finalStates)
		if(currentStates.find(finalState) != std::end(currentStates))
			return true;
	return false;
}

} /* namespace Automata */
