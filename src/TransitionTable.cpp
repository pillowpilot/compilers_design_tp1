#include "TransitionTable.h"

namespace Automata {

TransitionTable::TransitionTable()
:_table(), _symbols({Epsilon})
{
	_symbolsMapping.insert(std::make_pair(Epsilon, 0));
}

TransitionTable::TransitionTable(const TransitionTable& other)
:_table(other._table), _symbols(other._symbols), _symbolsMapping(other._symbolsMapping)
{
}

// Use pass-by-value to use copy-elision optimization
TransitionTable& TransitionTable::operator=(TransitionTable other)
{
	swap(other, *this);
	return *this;
}

StateType TransitionTable::addState()
{
	_table.emplace_back();
	for(size_t i = 0; i < _symbols.size(); i++)
		_table.back().emplace_back();
	return StateType(_table.size()-1);
}

void TransitionTable::addSymbol(const SymbolType& symbol)
{
	if(symbol.empty())
		throw std::invalid_argument("Empty symbol is not valid");

	if(_symbols.find(symbol) == std::end(_symbols))
	{
		_symbols.insert(symbol);
		_symbolsMapping.insert(std::make_pair(symbol, _symbolsMapping.size()));
		for(auto& row: _table)
			row.emplace_back();
	}
}

void TransitionTable::addTransition(const StateType& start, const SymbolType& symbol, const StateType& end)
{
	if(!isValidState(start) || !isValidState(end))
		throw std::invalid_argument("Invalid state");
	if(!isValidSymbol(symbol))
		throw std::invalid_argument("Invalid symbol");
	const size_t column = _symbolsMapping[symbol];
	_table.at(start).at(column).insert(end);
}

const StateSetType& TransitionTable::getTransition(const StateType& start, const SymbolType& symbol) const
{
	if(!isValidState(start))
		throw std::invalid_argument("Invalid state");
	if(!isValidSymbol(symbol))
		throw std::invalid_argument("Invalid symbol");
	const size_t column = _symbolsMapping.find(symbol)->second;
	return _table.at(start).at(column);
}

StateSetType& TransitionTable::getTransition(const StateType& start, const SymbolType& symbol)
{
	return const_cast<StateSetType&>(static_cast<const TransitionTable&>(*this).getTransition(start, symbol));
}

bool TransitionTable::isValidState(const StateType& state) const
{
	return 0 <= state && state < _table.size();
}

bool TransitionTable::isValidSymbol(const SymbolType& symbol) const
{
	return _symbols.find(symbol) != std::end(_symbols);
}

TransitionTable::TransitionIteratorTag TransitionTable::getTransitions(const StateType& state) const
{
	return TransitionIteratorTag(*this, state);
}


// StateIterator
TransitionTable::StateIterator::StateIterator(size_t maxIndex)
:_maxIndex(maxIndex), _currentIndex(0)
{
}

TransitionTable::StateIterator::StateIterator(size_t maxIndex, size_t currentIndex)
:_maxIndex(maxIndex), _currentIndex(currentIndex)
{
}

TransitionTable::StateIterator::StateIterator(const StateIterator& other)
:_maxIndex(other._maxIndex), _currentIndex(other._currentIndex)
{
}

TransitionTable::StateIterator& TransitionTable::StateIterator::operator=(StateIterator other)
{
	std::swap(_maxIndex, other._maxIndex);
	std::swap(_currentIndex, other._currentIndex);
	return *this;
}

StateType TransitionTable::StateIterator::operator*() const
{
	return static_cast<StateType>(_currentIndex);
}

void TransitionTable::StateIterator::operator++()
		{
	_currentIndex++;
		}

bool TransitionTable::StateIterator::operator==(const StateIterator& other) const
		{
	return _currentIndex == other._currentIndex;
		}

bool TransitionTable::StateIterator::operator!=(const StateIterator& other) const
		{
	return !operator==(other);
		}

// TransitionIterator
TransitionTable::TransitionIterator::TransitionIterator(const TransitionTable& transitionTable, const StateType& state)
:_transitionTable(&transitionTable), _state(state), _symbolIterator(std::begin(transitionTable._symbols))
{
}

TransitionTable::TransitionIterator::TransitionIterator(const TransitionTable& transitionTable, const StateType& state,
		const SymbolSetType::const_iterator& iterator)
:_transitionTable(&transitionTable), _state(state), _symbolIterator(iterator)
{
}

TransitionTable::TransitionIterator::TransitionIterator(const TransitionIterator& other)
:_transitionTable(other._transitionTable), _state(other._state), _symbolIterator(other._symbolIterator)
{
}

TransitionTable::TransitionIterator& TransitionTable::TransitionIterator::operator=(TransitionIterator rhs)
{
	std::swap(_transitionTable, rhs._transitionTable);
	std::swap(_state, rhs._state);
	std::swap(_symbolIterator, rhs._symbolIterator);
	return *this;
}

std::pair<SymbolType, StateSetType> TransitionTable::TransitionIterator::operator*() const
{
	const SymbolType symbol = *_symbolIterator;
	const StateSetType states = _transitionTable->getTransition(_state, symbol);
	return std::make_pair(symbol, states);
}

void TransitionTable::TransitionIterator::operator++()
{
	++_symbolIterator;
}

bool TransitionTable::TransitionIterator::operator==(const TransitionIterator& rhs) const
{
	return _transitionTable == rhs._transitionTable && _state == rhs._state && _symbolIterator == rhs._symbolIterator;
}

bool TransitionTable::TransitionIterator::operator!=(const TransitionIterator& rhs) const
{
	return !(*this == rhs);
}

// Friend methods
std::ostream& operator<<(std::ostream& os, const TransitionTable& tt)
{
	size_t maxStringLength = 0;
	std::vector<std::vector<std::string>> body;
	for(const auto& row: tt._table)
	{
		body.emplace_back();
		auto& body_row = body.back();
		for(const auto& transition: row)
		{
			const auto transitionString = to_string(transition);
			maxStringLength = std::max(maxStringLength, transitionString.size());
			body_row.push_back(transitionString);
		}
	}

	std::map<size_t, SymbolType> symbolMappingInverse;
	for(const auto& p: tt._symbolsMapping)
		symbolMappingInverse[p.second] = p.first;

	for(const auto& p: symbolMappingInverse)
	{
		os << std::setw(maxStringLength) << std::setfill(' ') << p.second;
	}
	os << std::endl;

	StateType state = 0;
	for(const auto& row: body)
	{
		os << std::setw(3) << std::setfill(' ') << state++;
		for(const auto& transition: row)
		{
			os << std::setw(maxStringLength) << std::setfill(' ') << transition;
		}
		os << std::endl;
	}

	return os;
}

void swap(TransitionTable& lhs, TransitionTable& rhs)
{
	std::swap(lhs._table, rhs._table);
	std::swap(lhs._symbols, rhs._symbols);
	std::swap(lhs._symbolsMapping, rhs._symbolsMapping);
}

TransitionTable::StateIterator begin(const TransitionTable& transitionTable)
{
	return TransitionTable::StateIterator(transitionTable._table.size());
}

TransitionTable::StateIterator end(const TransitionTable& transitionTable)
{
	const auto& numberOfStates = transitionTable._table.size();
	return TransitionTable::StateIterator(numberOfStates, numberOfStates);
}

TransitionTable::TransitionIterator begin(const TransitionTable::TransitionIteratorTag& transitionIteratorTag)
{
	const TransitionTable& transitionTable = transitionIteratorTag._transitionTable;
	const StateType& state = transitionIteratorTag._state;
	return TransitionTable::TransitionIterator(transitionTable, state);
}

TransitionTable::TransitionIterator end(const TransitionTable::TransitionIteratorTag& transitionIteratorTag)
{
	const TransitionTable& transitionTable = transitionIteratorTag._transitionTable;
	const StateType& state = transitionIteratorTag._state;
	return TransitionTable::TransitionIterator(transitionTable, state, std::end(transitionTable._symbols));
}

} /* namespace Automata */
