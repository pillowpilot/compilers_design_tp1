#include "Powerset.h"

namespace Automata {

DFA Powerset::apply(const NFA& nfa)
{
	using DFAStatesSet = std::set<StateSetType>;
	using DFAStatesQueue = std::queue<StateSetType>;

	const AlphabetType alphabet = getAlphabet(nfa);

	DFABuilder<StateSetType> builder;
	EpsilonClosure closure(nfa);

	const StateSetType sourceClosure = closure.getClosure(nfa.getInitialState());

	DFAStatesSet dfaStates;
	dfaStates.insert(sourceClosure);

	DFAStatesQueue dfaStatesToProcess;
	dfaStatesToProcess.push(sourceClosure);
	while(!dfaStatesToProcess.empty())
	{
		const auto dfaState = dfaStatesToProcess.front();
		dfaStatesToProcess.pop();

		StateSetType sources;
		for(const auto& symbol: alphabet)
		{
			const auto nextDFAState = closure.getClosure(multipleMove(nfa, dfaState, symbol));
			if(dfaStates.find(nextDFAState) == std::end(dfaStates))
			{
				dfaStates.insert(nextDFAState);
				dfaStatesToProcess.push(nextDFAState);
			}
			builder.addTransition(dfaState, symbol, nextDFAState);
		}
	}

	builder.setInitialStateLabel(sourceClosure);

	for(const auto& dfaState: dfaStates)
		if(containsAFinalState(dfaState, nfa.getFinalStates()))
			builder.addFinalStateLabel(dfaState);

	return builder.build();

}

StateSetType Powerset::moveOverSet(const NFA& nfa, const StateSetType& sources, const SymbolType& symbol)
{
	StateSetType targets;
	for(const auto& source: sources)
	{
		const StateSetType singleSourceTransitions= nfa.move(source, symbol);
		for(const auto& transition: singleSourceTransitions)
			targets.insert(transition);
	}

	return targets;
}

AlphabetType Powerset::getAlphabet(const NFA& nfa)
{
	AlphabetType alphabet;

	for(const auto& state: nfa)
		for(const auto& p: nfa.getTransitions(state))
		{
			const SymbolType symbol = p.first;
			if(symbol != Epsilon)
				alphabet.insert(symbol);
		}

	return alphabet;
}

StateSetType Powerset::multipleMove(const NFA& nfa, const StateSetType& sources, const SymbolType& symbol)
{
	StateSetType targets;
	for(const auto& state: sources)
	{
		const auto singleSourceTargets = nfa.move(state, symbol);
		for(const auto& state: singleSourceTargets)
			targets.insert(state);
	}
	return targets;
}

bool Powerset::containsAFinalState(const StateSetType& states, const StateSetType& finalStates)
{
	for(const auto& finalState: finalStates)
		if(states.find(finalState) != std::end(states))
			return true;
	return false;
}

} /* namespace Automata */
