#include "gtest/gtest.h"
#include "TransitionTable.h"

#include <iostream>

using Automata::Epsilon;
using Automata::StateType;
using Automata::StateSetType;
using Automata::SymbolType;
using Automata::TransitionTable;


TEST(TransitionTableTest, AddState)
{
	TransitionTable tt;
	StateType newState = tt.addState();
	ASSERT_TRUE(tt.isValidState(newState));
}

TEST(TransitionTable, AddSymbolAndIsValidSymbol)
{
	TransitionTable tt;
	tt.addSymbol(SymbolType("a"));
	ASSERT_TRUE(tt.isValidSymbol("a"));
	ASSERT_FALSE(tt.isValidSymbol("b"));
}

TEST(TransitionTable, ExceptionOnEmpty)
{
	TransitionTable tt;
	ASSERT_ANY_THROW(tt.addSymbol(""));
}

TEST(TransitionTable, AddTransitionAndGetTransition)
{
	TransitionTable tt;
	const auto source = tt.addState();
	const auto end = tt.addState();
	const auto symbol1 = "a";
	const auto symbol2 = "b";
	tt.addSymbol(symbol1);
	tt.addSymbol(symbol2);

	ASSERT_ANY_THROW(tt.addTransition(123, symbol1, end));
	ASSERT_ANY_THROW(tt.addTransition(source, "c", end));
	ASSERT_ANY_THROW(tt.addTransition(source, symbol1, 123));

	tt.addTransition(source, symbol1, end);
	tt.addTransition(source, symbol2, end);

	ASSERT_EQ(tt.getTransition(source, symbol1), StateSetType({end}));
	ASSERT_EQ(tt.getTransition(source, symbol2), StateSetType({end}));
	ASSERT_ANY_THROW(tt.getTransition(source, "c"));
}

TEST(TransitionTableTest, Iterator)
{
	TransitionTable tt;
	StateSetType states;
	for(int i = 0; i < 10; i ++)
		states.insert(tt.addState());

	for(const auto& state: tt)
		ASSERT_TRUE(states.find(state) != std::end(states));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
