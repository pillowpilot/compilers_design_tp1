#include "gtest/gtest.h"
#include <NFA.h>

using namespace Automata;

TEST(EpsilonClosure, kleeneLike)
{
	NFABuilder<int> builder;

	builder.addTransition(0, Epsilon, 1);
	builder.addTransition(1, "a", 2);
	builder.addTransition(2, Epsilon, 3);
	builder.addTransition(0, Epsilon, 3);
	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(3);

	const NFA nfa = builder.build();
	EpsilonClosure closure(nfa);

	ASSERT_EQ(StateSetType({0, 1, 3}), closure.getClosure(nfa.getInitialState()));
	ASSERT_EQ(StateSetType({1}), closure.getClosure(1));
	ASSERT_EQ(StateSetType({2, 3}), closure.getClosure(2));
	ASSERT_EQ(StateSetType({3}), closure.getClosure(3));
}

TEST(EpsilonClosure, withCycles)
{
	NFABuilder<int> builder;

	builder.addTransition(0, Epsilon, 0);
	builder.addTransition(0, Epsilon, 1);
	builder.addTransition(1, Epsilon, 2);
	builder.addTransition(0, Epsilon, 2);
	builder.addTransition(2, Epsilon, 0);
	builder.addTransition(0, "a", 3);
	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(3);

	const NFA nfa = builder.build();
	EpsilonClosure closure(nfa);

	ASSERT_EQ(StateSetType({0, 1, 2}), closure.getClosure(0));
	ASSERT_EQ(StateSetType({0, 1, 2}), closure.getClosure(1));
	ASSERT_EQ(StateSetType({0, 1, 2}), closure.getClosure(2));
	ASSERT_EQ(StateSetType({3}), closure.getClosure(3));
}

TEST(EpsilonClosure, multipleSources)
{
	NFABuilder<int> builder;

	builder.addTransition(0, Epsilon, 0);
	builder.addTransition(0, Epsilon, 1);
	builder.addTransition(1, Epsilon, 2);
	builder.addTransition(0, Epsilon, 2);
	builder.addTransition(2, Epsilon, 0);
	builder.addTransition(0, "a", 3);
	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(3);

	const NFA nfa = builder.build();
	EpsilonClosure closure(nfa);

	ASSERT_EQ(StateSetType({0, 1, 2}), closure.getClosure(0));
	ASSERT_EQ(StateSetType({0, 1, 2}), closure.getClosure(1));
	ASSERT_EQ(StateSetType({0, 1, 2}), closure.getClosure(2));
	ASSERT_EQ(StateSetType({3}), closure.getClosure(3));

	ASSERT_EQ(closure.getClosure({0, 1}), StateSetType({0, 1, 2}));
	ASSERT_EQ(closure.getClosure({0, 3}), StateSetType({0, 1, 2, 3}));
	ASSERT_EQ(closure.getClosure({0, 1, 2}), StateSetType({0, 1, 2}));
	ASSERT_EQ(closure.getClosure({3}), StateSetType({3}));
}

TEST(NFARunner, simple1)
{
	// a*
	NFABuilder<int> builder;
	builder.addTransition(0, Epsilon, 1);
	builder.addTransition(1, "a", 2);
	builder.addTransition(2, Epsilon, 3);
	builder.addTransition(0, Epsilon, 3);
	builder.addTransition(2, Epsilon, 1);
	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(3);

	NFARunner runner(builder.build());

	const std::set<std::string> correctInputs({"", "a", "aa", "aaa"});
	for(const auto& input: correctInputs)
		ASSERT_TRUE(runner.run(input));

	const std::set<std::string> incorrectInputs({"b", "c", "bc"});
	for(const auto& input: incorrectInputs)
		ASSERT_FALSE(runner.run(input));
}

TEST(NFARunner, simple2)
{
	// a?
	NFABuilder<int> builder;
	builder.addTransition(0, Epsilon, 1);
	builder.addTransition(1, "a", 2);
	builder.addTransition(2, Epsilon, 3);
	builder.addTransition(0, Epsilon, 3);
	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(3);

	NFARunner runner(builder.build());

	const std::set<std::string> correctInputs({"", "a"});
	for(const auto& input: correctInputs)
		ASSERT_TRUE(runner.run(input));

	const std::set<std::string> incorrectInputs({"aa", "aaa", "b", "c", "bc"});
	for(const auto& input: incorrectInputs)
		ASSERT_FALSE(runner.run(input));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
