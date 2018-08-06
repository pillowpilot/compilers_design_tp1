#include "Thompson.h"

namespace Automata {

Automata::NFA Trivial::apply(const SymbolType& symbol)
{
	Automata::NFABuilder<int> builder;

	builder.addTransition(0, symbol, 1);
	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(1);

	return builder.build();
}

Automata::NFA Concatenation::apply(const NFA& first, const NFA& second)
{
	const std::string firstPrefix("_first");
	const std::string secondPrefix("_second");

	const StateType firstStartState = first.getInitialState();
	const StateType firstEndState = *std::begin(first.getFinalStates());
	const StateType secondStartState = second.getInitialState();
	const StateType secondEndState = *std::begin(second.getFinalStates());

	Automata::NFABuilder<std::string> builder;

	copyTransitions(builder, first, firstPrefix);

	builder.addTransition(firstPrefix+std::to_string(firstEndState), Epsilon, secondPrefix+std::to_string(secondStartState));

	copyTransitions(builder, second, secondPrefix);

	builder.setInitialStateLabel(firstPrefix + std::to_string(firstStartState));
	builder.addFinalStateLabel(secondPrefix + std::to_string(secondEndState));

	return builder.build();
}

Automata::NFA Alternative::apply(const NFA& first, const NFA& second)
{
	const std::string firstPrefix("_first");
	const std::string secondPrefix("_second");

	const std::string startStateLabel = "_begining";
	const std::string finalStateLabel = "_final";
	const StateType firstStartState = first.getInitialState();
	const StateType firstEndState = *std::begin(first.getFinalStates());
	const StateType secondStartState = second.getInitialState();
	const StateType secondEndState = *std::begin(second.getFinalStates());

	Automata::NFABuilder<std::string> builder;

	builder.addTransition(startStateLabel, Epsilon, firstPrefix + std::to_string(firstStartState));
	copyTransitions(builder, first, firstPrefix);
	builder.addTransition(firstPrefix + std::to_string(firstEndState), Epsilon, finalStateLabel);

	builder.addTransition(startStateLabel, Epsilon, secondPrefix + std::to_string(secondStartState));
	copyTransitions(builder, second, secondPrefix);
	builder.addTransition(secondPrefix + std::to_string(secondEndState), Epsilon, finalStateLabel);

	builder.setInitialStateLabel(startStateLabel);
	builder.addFinalStateLabel(finalStateLabel);

	return builder.build();
}

Automata::NFA Kleene::apply(const Automata::NFA& nfa)
{
	const std::string startStateLabel = "_begining";
	const std::string finalStateLabel = "_final";
	const StateType nfaStartState = nfa.getInitialState();
	const StateType nfaEndState = *std::begin(nfa.getFinalStates());

	NFABuilder<std::string> builder;

	copyTransitions(builder, nfa);

	builder.addTransition(startStateLabel, Epsilon, std::to_string(nfaStartState));
	builder.addTransition(startStateLabel, Epsilon, finalStateLabel);
	builder.addTransition(std::to_string(nfaEndState), Epsilon, finalStateLabel);
	builder.addTransition(std::to_string(nfaEndState), Epsilon, std::to_string(nfaStartState));

	builder.setInitialStateLabel(startStateLabel);
	builder.addFinalStateLabel(finalStateLabel);

	return builder.build();
}

void copyTransitions(Automata::NFABuilder<std::string>& builder, const NFA& nfa, const std::string& prefix)
{
	for(const auto& stateId: nfa)
	{
		const std::string startStateLabel = prefix + std::to_string(stateId);
		for(const auto& p: nfa.getTransitions(stateId))
		{
			const auto symbol = p.first;
			const auto transition = p.second;
			for(const auto& endStateId: transition)
			{
				const std::string endStateLabel = prefix + std::to_string(endStateId);
				builder.addTransition(startStateLabel, symbol, endStateLabel);
			}
		}
	}
}

Automata::NFA Thompson::apply(const std::string& postfix)
{
	std::stack<NFA> output;

	for(const auto& c: postfix)
	{
		if(isConcatenationOperator(c))
		{
			const NFA b = output.top(); output.pop();
			const NFA a = output.top(); output.pop();
			output.push(Concatenation::apply(a, b));
		}
		else if(isAlternativeOperator(c))
		{
			const NFA b = output.top(); output.pop();
			const NFA a = output.top(); output.pop();
			output.push(Alternative::apply(a, b));
		}
		else if(isKleeneOperator(c))
		{
			const NFA a = output.top(); output.pop();
			output.push(Kleene::apply(a));
		}
		else
		{
			output.push(Trivial::apply(std::string(1, c)));
		}
	}

	return output.top();
}

} /* namespace Automata */
