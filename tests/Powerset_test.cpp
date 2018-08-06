#include "gtest/gtest.h"
#include "Powerset.h"

using namespace Automata;

TEST(Powerset, allABstringWithABBsufix)
{
	NFABuilder<int> builder;
	builder.addTransition(0, Epsilon, 1);
	builder.addTransition(0, Epsilon, 7);
	builder.addTransition(1, Epsilon, 2);
	builder.addTransition(1, Epsilon, 4);
	builder.addTransition(2, "a", 3);
	builder.addTransition(3, Epsilon, 6);
	builder.addTransition(4, "b", 5);
	builder.addTransition(5, Epsilon, 6);
	builder.addTransition(6, Epsilon, 1);
	builder.addTransition(6, Epsilon, 7);
	builder.addTransition(7, "a", 8);
	builder.addTransition(8, "b", 9);
	builder.addTransition(9, "b", 10);
	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(10);
	const NFA nfa = builder.build();

	NFARunner nfaRunner(nfa);
	ASSERT_TRUE(nfaRunner.run("abb"));
	ASSERT_TRUE(nfaRunner.run("aabb"));
	ASSERT_TRUE(nfaRunner.run("babb"));
	ASSERT_TRUE(nfaRunner.run("ababb"));
	ASSERT_TRUE(nfaRunner.run("aaabababb"));
	ASSERT_FALSE(nfaRunner.run(""));
	ASSERT_FALSE(nfaRunner.run("a"));
	ASSERT_FALSE(nfaRunner.run("bb"));
	ASSERT_FALSE(nfaRunner.run("abab"));

	const DFA dfa = Powerset::apply(nfa);

	DFARunner dfaRunner(dfa);
	ASSERT_TRUE(dfaRunner.run("abb"));
	ASSERT_TRUE(dfaRunner.run("aabb"));
	ASSERT_TRUE(dfaRunner.run("babb"));
	ASSERT_TRUE(dfaRunner.run("ababb"));
	ASSERT_TRUE(dfaRunner.run("aaabababb"));
	ASSERT_FALSE(dfaRunner.run(""));
	ASSERT_FALSE(dfaRunner.run("a"));
	ASSERT_FALSE(dfaRunner.run("bb"));
	ASSERT_FALSE(dfaRunner.run("abab"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
