#include "gtest/gtest.h"
#include "Hopcroft.h"
#include "DFA.h"

using namespace Automata;

TEST(Hopcroft, minDFAasInput)
{
	DFABuilder<int> builder;
	builder.addTransition(0, "a", 1);
	builder.addTransition(0, "b", 0);
	builder.addTransition(1, "b", 2);
	builder.addTransition(1, "a", 1);
	builder.addTransition(2, "a", 1);
	builder.addTransition(2, "b", 3);
	builder.addTransition(3, "a", 1);
	builder.addTransition(3, "b", 0);
	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(3);
	const DFA dfa = builder.build();

	DFARunner dfaRunner(dfa);
	ASSERT_TRUE(dfaRunner.run("abb"));
	ASSERT_TRUE(dfaRunner.run("abbabb"));
	ASSERT_TRUE(dfaRunner.run("ababb"));
	ASSERT_TRUE(dfaRunner.run("babb"));
	ASSERT_TRUE(dfaRunner.run("ababbabb"));
	ASSERT_FALSE(dfaRunner.run("ab"));
	ASSERT_FALSE(dfaRunner.run("bab"));
	ASSERT_FALSE(dfaRunner.run("b"));
	ASSERT_FALSE(dfaRunner.run("a"));
	ASSERT_FALSE(dfaRunner.run("abbaaba"));

	const DFA minDfa = Hopcroft::apply(dfa);

	DFARunner minDfaRunner(dfa);
	ASSERT_TRUE(minDfaRunner.run("abb"));
	ASSERT_TRUE(minDfaRunner.run("abbabb"));
	ASSERT_TRUE(minDfaRunner.run("ababb"));
	ASSERT_TRUE(minDfaRunner.run("babb"));
	ASSERT_TRUE(minDfaRunner.run("ababbabb"));
	ASSERT_FALSE(minDfaRunner.run("ab"));
	ASSERT_FALSE(minDfaRunner.run("bab"));
	ASSERT_FALSE(minDfaRunner.run("b"));
	ASSERT_FALSE(minDfaRunner.run("a"));
	ASSERT_FALSE(minDfaRunner.run("abbaaba"));
}

TEST(Hopcroft, allABStringStartingWithB)
{
	DFABuilder<int> builder;
	builder.addTransition(0, "a", 1);
	builder.addTransition(0, "b", 2);
	builder.addTransition(1, "b", 1);
	builder.addTransition(1, "a", 1);
	builder.addTransition(2, "a", 2);
	builder.addTransition(2, "b", 2);
	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(2);
	const DFA dfa = builder.build();

	DFARunner dfaRunner(dfa);
	ASSERT_TRUE(dfaRunner.run("babb"));
	ASSERT_TRUE(dfaRunner.run("babbabbbaa"));
	ASSERT_TRUE(dfaRunner.run("bababab"));
	ASSERT_TRUE(dfaRunner.run("babb"));
	ASSERT_TRUE(dfaRunner.run("b"));
	ASSERT_FALSE(dfaRunner.run(""));
	ASSERT_FALSE(dfaRunner.run("ababa"));
	ASSERT_FALSE(dfaRunner.run("aab"));
	ASSERT_FALSE(dfaRunner.run("a"));
	ASSERT_FALSE(dfaRunner.run("abbaaba"));

	const DFA minDfa = Hopcroft::apply(dfa);

	DFARunner minDfaRunner(dfa);
	ASSERT_TRUE(dfaRunner.run("babb"));
	ASSERT_TRUE(dfaRunner.run("babbabbbaa"));
	ASSERT_TRUE(dfaRunner.run("bababab"));
	ASSERT_TRUE(dfaRunner.run("babb"));
	ASSERT_TRUE(dfaRunner.run("b"));
	ASSERT_FALSE(dfaRunner.run(""));
	ASSERT_FALSE(dfaRunner.run("ababa"));
	ASSERT_FALSE(dfaRunner.run("aab"));
	ASSERT_FALSE(dfaRunner.run("a"));
	ASSERT_FALSE(dfaRunner.run("abbaaba"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
