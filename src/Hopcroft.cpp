#include "Hopcroft.h"

namespace Automata {

DFA Hopcroft::apply(const DFA& dfa)
{
	const auto alphabet = getAlphabet(dfa);
	PartitionType partition = initialPartition(dfa);

	PartitionType newPartition = improvePartition(dfa, partition, alphabet);
	while(newPartition != partition)
	{
		partition = newPartition;
		newPartition = improvePartition(dfa, partition, alphabet);
	}

	DFABuilder<IdType> builder;
	// Add transitions
	for(const auto& group: partition)
	{
		const auto& representativeState = *group.states.begin();
		const auto& sourceLabel = group.id;
		for(const auto& symbol: alphabet)
		{
			const auto& targetState = dfa.move(representativeState, symbol);
			const auto& targetLabel = findGroupId(partition, targetState);

			builder.addTransition(sourceLabel, symbol, targetLabel);
		}
	}
	// Find group with source state and set it to start state into the min DFA
	for(const auto& group: partition)
		if(group.states.find(dfa.getInitialState()) != std::end(group.states))
		{
			builder.setInitialStateLabel(group.id);
			break;
		}
	// Find all groups with at least one final state and add them into the min DFA
	const auto finalStates = dfa.getFinalStates();
	for(const auto& group: partition)
		for(const auto& finalState: finalStates)
			if(group.states.find(finalState) != std::end(group.states))
				builder.addFinalStateLabel(group.id);

	return builder.build();
}

Hopcroft::PartitionType Hopcroft::initialPartition(const DFA& dfa)
{
	StateSetType acceptedStates = dfa.getFinalStates();
	StateSetType nonAcceptedStates;
	for(const auto& state: dfa)
		if(acceptedStates.find(state) == std::end(acceptedStates))
			nonAcceptedStates.insert(state);

	PartitionType partition;
	partition.insert({acceptedStates, 0});
	partition.insert({nonAcceptedStates, 1});

	return partition;
}

Hopcroft::PartitionType Hopcroft::improvePartition(const DFA& dfa, const PartitionType& partition, const AlphabetType& alphabet)
{
	IdType groupIdCounter = 0;
	PartitionType newPartition;

	for(const auto& group: partition)
	{
		bool copyOriginalGroupToNewPartition = true;
		for(const auto& symbol: alphabet)
		{
			std::map<IdType, StateSetType> newGroups;

			for(const auto& state: group.states)
			{
				const auto nextState = dfa.move(state, symbol);
				const auto groupId = findGroupId(partition, nextState);
				newGroups[groupId].insert(state);
			}

			if(newGroups.size() > 1)
			{
				for(const auto& p: newGroups)
				{
					const auto& states = p.second;
					const auto& groupId = groupIdCounter++;
					newPartition.insert({states, groupId});
				}
				copyOriginalGroupToNewPartition = false;
				break;
			}
		}
		if(copyOriginalGroupToNewPartition)
			newPartition.insert({group.states, groupIdCounter++});
	}

	return newPartition;
}

AlphabetType Hopcroft::getAlphabet(const DFA& dfa)
{
	AlphabetType alphabet;
	for(const auto& state: dfa)
	{
		for(const auto& transition: dfa.getTransitions(state))
		{
			const auto symbol = transition.first;
			if(symbol != Epsilon)
				alphabet.insert(symbol);
		}
	}
	return alphabet;
}

Hopcroft::IdType Hopcroft::findGroupId(const PartitionType& partition, const StateType& state)
{
	for(const auto& group: partition)
		for(const auto& groupState: group.states)
			if(state == groupState)
				return group.id;
	throw std::invalid_argument("State is not contained in this partition.");
}

} /* namespace Automata */
