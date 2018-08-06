#include "gtest/gtest.h"
#include "Thompson.h"

using namespace Automata;

TEST(TrivialConstruction, simple1)
{
	const SymbolType symbol("a");

	const NFA nfa = Trivial::apply(symbol);

	NFARunner runner(nfa);

	ASSERT_TRUE(runner.run(symbol));
	ASSERT_FALSE(runner.run(""));
	ASSERT_FALSE(runner.run("aa"));
	ASSERT_FALSE(runner.run("ab"));
	ASSERT_FALSE(runner.run("b"));
}

TEST(TrivialConstruction, Epsilon)
{
	const NFA nfa = Trivial::apply(Epsilon);

	NFARunner runner(nfa);

	ASSERT_TRUE(runner.run(""));
	ASSERT_FALSE(runner.run("a"));
	ASSERT_FALSE(runner.run("aa"));
	ASSERT_FALSE(runner.run("ab"));
	ASSERT_FALSE(runner.run("b"));
}

TEST(ConcatenationConstrction, simple1)
{
	const SymbolType symbol1("a");
	const SymbolType symbol2("b");

	const NFA nfa = Concatenation::apply(Trivial::apply(symbol1), Trivial::apply(symbol2));
	NFARunner runner(nfa);

	ASSERT_TRUE(runner.run(symbol1 + symbol2));
	ASSERT_FALSE(runner.run(symbol1));
	ASSERT_FALSE(runner.run(symbol2));
	ASSERT_FALSE(runner.run(symbol2 + symbol1));
	ASSERT_FALSE(runner.run("ccab"));
	ASSERT_FALSE(runner.run("abcc"));
	ASSERT_FALSE(runner.run(symbol1+symbol2+symbol1+symbol2));
}

TEST(ConcatenationConstruction, Epsilon1)
{
	const SymbolType symbol1("a");

	const NFA nfa = Concatenation::apply(Trivial::apply(Epsilon), Trivial::apply(symbol1));
	NFARunner runner(nfa);

	ASSERT_TRUE(runner.run(symbol1));
	ASSERT_FALSE(runner.run(""));
	ASSERT_FALSE(runner.run("aa"));
}

TEST(ConcatenationConstruction, Epsilon2)
{
	const SymbolType symbol1("a");

	const NFA nfa = Concatenation::apply(Trivial::apply(symbol1), Trivial::apply(Epsilon));
	NFARunner runner(nfa);

	ASSERT_TRUE(runner.run(symbol1));
	ASSERT_FALSE(runner.run(""));
	ASSERT_FALSE(runner.run("aa"));
}

TEST(AlternativeConstruction, simple1)
{
	const SymbolType symbol1("a");
	const SymbolType symbol2("b");

	const NFA nfa = Alternative::apply(Trivial::apply(symbol1), Trivial::apply(symbol2));
	NFARunner runner(nfa);

	ASSERT_TRUE(runner.run(symbol1));
	ASSERT_TRUE(runner.run(symbol2));
	ASSERT_FALSE(runner.run(symbol2 + symbol1));
	ASSERT_FALSE(runner.run("ccab"));
	ASSERT_FALSE(runner.run("abcc"));
	ASSERT_FALSE(runner.run(symbol1+symbol2+symbol1+symbol2));
}

TEST(AlternativeConstruction, epsilon)
{
	const SymbolType symbol1("a");

	const NFA nfa = Alternative::apply(Trivial::apply(symbol1), Trivial::apply(Epsilon));
	NFARunner runner(nfa);

	ASSERT_TRUE(runner.run(symbol1));
	ASSERT_TRUE(runner.run(""));
	ASSERT_FALSE(runner.run("ccab"));
	ASSERT_FALSE(runner.run("abcc"));
	ASSERT_FALSE(runner.run("aa"));
}

TEST(KleeneConstruction, simple1)
{
	const SymbolType symbol("a");

	const NFA nfa = Kleene::apply(Trivial::apply(symbol));
	NFARunner runner(nfa);

	ASSERT_TRUE(runner.run(""));
	ASSERT_TRUE(runner.run(symbol));
	ASSERT_TRUE(runner.run(symbol+symbol));
	ASSERT_TRUE(runner.run(symbol+symbol+symbol));
	ASSERT_FALSE(runner.run(symbol+"b"+symbol));
	ASSERT_FALSE(runner.run(symbol+symbol+"b"));
}

TEST(KleeneConstruction, epsilon)
{
	const NFA nfa = Kleene::apply(Trivial::apply(Epsilon));
	NFARunner runner(nfa);

	ASSERT_TRUE(runner.run(""));
	ASSERT_FALSE(runner.run("a"));
	ASSERT_FALSE(runner.run("aa"));
}

TEST(ThompsonConstruction, simple1)
{
	const std::string postfixExpression = "ab.";
	const NFA nfa = Thompson::apply(postfixExpression);
	NFARunner runner(nfa);

	ASSERT_TRUE(runner.run("ab"));
	ASSERT_FALSE(runner.run("a"));
	ASSERT_FALSE(runner.run("aa"));
	ASSERT_FALSE(runner.run("aba"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
