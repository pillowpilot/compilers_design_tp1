#include "gtest/gtest.h"
#include "SimpleAlgorithm.h"

TEST(ShuntingYardTests, OneOrLessOperators)
{
	ASSERT_EQ("a", ShuntingYard::SimpleAlgorithm::apply("a"));
	ASSERT_EQ("a", ShuntingYard::SimpleAlgorithm::apply("(a)"));
	ASSERT_EQ("a*", ShuntingYard::SimpleAlgorithm::apply("a*"));
	ASSERT_EQ("ab|", ShuntingYard::SimpleAlgorithm::apply("a|b"));
	ASSERT_EQ("ab.", ShuntingYard::SimpleAlgorithm::apply("a.b"));
}

TEST(ShuntingYardTests, OperatorsRelations)
{
	ASSERT_EQ("ab|*", ShuntingYard::SimpleAlgorithm::apply("(a|b)*"));
	ASSERT_EQ("ab.*", ShuntingYard::SimpleAlgorithm::apply("(a.b)*"));
	ASSERT_EQ("a**", ShuntingYard::SimpleAlgorithm::apply("a**"));
	ASSERT_EQ("ab|c|", ShuntingYard::SimpleAlgorithm::apply("a|b|c"));
	ASSERT_EQ("ab.c.*", ShuntingYard::SimpleAlgorithm::apply("(a.b.c)*"));
	ASSERT_EQ("a*b*|", ShuntingYard::SimpleAlgorithm::apply("a*|b*"));
	ASSERT_EQ("a*b*.", ShuntingYard::SimpleAlgorithm::apply("a*.b*"));
	ASSERT_EQ("ab*|", ShuntingYard::SimpleAlgorithm::apply("a|b*"));

}

TEST(ShuntingYardTests, LeftAssociativity)
{
	ASSERT_EQ("ab|c|", ShuntingYard::SimpleAlgorithm::apply("a|b|c"));
	ASSERT_NE("abc||", ShuntingYard::SimpleAlgorithm::apply("a|b|c"));
	ASSERT_EQ("ab.c.", ShuntingYard::SimpleAlgorithm::apply("a.b.c"));
	ASSERT_NE("abc..", ShuntingYard::SimpleAlgorithm::apply("a.b.c"));

}

TEST(ShuntingYardTests, ComplexOnes)
{
	ASSERT_EQ("ab|*a.b.b.", ShuntingYard::SimpleAlgorithm::apply("(a|b)*.a.b.b"));
	ASSERT_EQ("abc*|.", ShuntingYard::SimpleAlgorithm::apply("a.(b|c*)"));
	ASSERT_EQ("abc|*.", ShuntingYard::SimpleAlgorithm::apply("a.(b|c)*"));
	ASSERT_EQ("aa.b|*abb.|*.", ShuntingYard::SimpleAlgorithm::apply("(a.a|b)*.(a|b.b)*"));

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}
