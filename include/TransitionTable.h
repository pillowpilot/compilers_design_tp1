#ifndef TRANSITIONTABLE_H_
#define TRANSITIONTABLE_H_

#include <vector>
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <iomanip>

#include "Common.h"

namespace Automata
{

class TransitionTable
{
private:
	using TableType = std::vector<std::vector<StateSetType>>;
	using SymbolToIndexType = std::map<SymbolType, size_t>;

	TableType _table;
	SymbolSetType _symbols;
	SymbolToIndexType _symbolsMapping;

public:
	TransitionTable();
	TransitionTable(const TransitionTable&);
	TransitionTable& operator=(TransitionTable);
	StateType addState();
	void addSymbol(const SymbolType&);
	void addTransition(const StateType&, const SymbolType&, const StateType&);
	const StateSetType& getTransition(const StateType&, const SymbolType&) const;
	StateSetType& getTransition(const StateType&, const SymbolType&);
	bool isValidState(const StateType&) const;
	bool isValidSymbol(const SymbolType&) const;
	~TransitionTable() = default;
	// Iterators
	class StateIterator
	{
	private:
		size_t _maxIndex;
		size_t _currentIndex;

	public:
		StateIterator(size_t);
		StateIterator(size_t, size_t);
		StateIterator(const StateIterator&);
		StateIterator& operator=(StateIterator);
		StateType operator*() const;
		void operator++();
		bool operator==(const StateIterator&) const;
		bool operator!=(const StateIterator&) const;
		~StateIterator() = default;
	};
	class TransitionIterator
	{
	private:
		const TransitionTable* _transitionTable;
		StateType _state;
		SymbolSetType::const_iterator _symbolIterator;

	public:
		TransitionIterator(const TransitionTable&, const StateType&);
		TransitionIterator(const TransitionTable&, const StateType&, const SymbolSetType::const_iterator&);
		TransitionIterator(const TransitionIterator&);
		TransitionIterator& operator=(TransitionIterator);
		std::pair<SymbolType, StateSetType> operator*() const;
		void operator++();
		bool operator==(const TransitionIterator&) const;
		bool operator!=(const TransitionIterator&) const;
		~TransitionIterator() = default;
	};
	class TransitionIteratorTag
	{
	public:
		const TransitionTable& _transitionTable;
		const StateType& _state;
		TransitionIteratorTag(const TransitionTable& transitionTable, const StateType& state)
		:_transitionTable(transitionTable), _state(state)
		{}
	};
	TransitionIteratorTag getTransitions(const StateType&) const;
	// Friends functions
	friend std::ostream& operator<<(std::ostream&, const TransitionTable&);
	friend void swap(TransitionTable& lhs, TransitionTable& rhs);
	friend StateIterator begin(const TransitionTable&);
	friend StateIterator end(const TransitionTable&);
	friend TransitionIterator begin(const TransitionIteratorTag&);
	friend TransitionIterator end(const TransitionIteratorTag&);
};

} /* namespace Automata */

#endif /* TRANSITIONTABLE_H_ */
