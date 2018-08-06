#include "gtest/gtest.h"
#include "DFA.h"

using namespace Automata;

TEST(DFABuilder, simple1)
{
	DFABuilder<int> builder;
	builder.addTransition(0, "a", 1);
	builder.addTransition(1, "b", 2);
	builder.addTransition(0, "b", 3);
	builder.addTransition(1, "a", 3);
	builder.addTransition(3, "a", 3);
	builder.addTransition(3, "b", 3);
	builder.addTransition(2, "b", 3);
	builder.addTransition(2, "a", 1);

	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(2);

	const DFA dfa = builder.build(); // (a.b)+

	DFARunner runner(dfa);

	ASSERT_TRUE(runner.run("ab"));
	ASSERT_TRUE(runner.run("abab"));
	ASSERT_TRUE(runner.run("ababab"));
	ASSERT_FALSE(runner.run("aba"));
	ASSERT_FALSE(runner.run("abaa"));
	ASSERT_FALSE(runner.run("a"));
	ASSERT_FALSE(runner.run("bbb"));
	ASSERT_FALSE(runner.run("baba"));
	ASSERT_FALSE(runner.run(""));
}

TEST(DFARunner, simple2)
{
	DFABuilder<int> builder;
	builder.addTransition(0, "a", 1);
	builder.addTransition(1, "b", 2);
	builder.addTransition(2, "b", 3);
	builder.addTransition(0, "b", 0);
	builder.addTransition(1, "a", 1);
	builder.addTransition(2, "a", 1);
	builder.addTransition(3, "a", 1);

	builder.setInitialStateLabel(0);
	builder.addFinalStateLabel(3);

	const DFA dfa = builder.build(); //(a|b)*.a.b.b

	DFARunner runner(dfa);

	ASSERT_TRUE(runner.run("abb"));
	ASSERT_TRUE(runner.run("aabb"));
	ASSERT_TRUE(runner.run("ababb"));
	ASSERT_TRUE(runner.run("bbabb"));
	ASSERT_TRUE(runner.run("abbabb"));
	ASSERT_FALSE(runner.run("abba"));
	ASSERT_FALSE(runner.run("abbb"));
	ASSERT_FALSE(runner.run("ba"));
	ASSERT_FALSE(runner.run("a"));
	ASSERT_FALSE(runner.run("b"));
	ASSERT_FALSE(runner.run("bb"));
	ASSERT_FALSE(runner.run("ab"));

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
