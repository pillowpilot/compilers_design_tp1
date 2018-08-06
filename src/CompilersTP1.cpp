#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "Hopcroft.h"
#include "SimpleAlgorithm.h"
#include "Powerset.h"
#include "Thompson.h"
#include "DFA.h"

const std::string exitToken = "$";

void printIntroduction()
{
	//std::string input = "(a|b)*.a.b.b";
	//std::string input = "a.(a|b)*.a.b.b";
	//std::string input = "f.e.d.e.r.i.c.o";
	std::cout << "Available operators: " << std::endl;
	std::cout << "1. Parenthesis, <(> and <)>" << std::endl;
	std::cout << "2. Kleene, <*>" << std::endl;
	std::cout << "3. Alternative, <|>" << std::endl;
	std::cout << "4. Concatenation, <.>" << std::endl;
	std::cout << std::endl;
	std::cout << "Some examples: " << std::endl;
	std::cout << "(a|b)*.a.b.b -- All strings ending with abb" << std::endl;
	std::cout << "a.(a|b)*.b -- All string starting with a and ending with b" << std::endl;
	std::cout << "f.e.d.e.r.i.c.o -- Only the string federico" << std::endl;
	std::cout << std::endl;
	std::cout << "Some observations:" << std::endl;
	std::cout << "Alphabet will be calculated from the input" << std::endl;
	std::cout << "The concatenation operator is *explicitly* symbolized with <.>" << std::endl;
	std::cout << "The string <" << exitToken << "> must not belong to the alphabet" << std::endl;
	std::cout << std::endl;
}

void printConclution()
{
	std::cout << "Exiting..." << std::endl;
}

void testDFA(const Automata::DFA& dfa)
{
	Automata::DFARunner runner(dfa);

	std::string input;
	do
	{
		std::cout << "Input string (" << exitToken << " to exit, " << Automata::Epsilon << " to test empty string): " << std::endl;
		std::cin >> input;
		if(input == Automata::Epsilon)
			input = "";
		if(input != exitToken)
		{
			if(runner.run(input))
				std::cout << "Input <" << input << "> acepted." << std::endl;
			else
				std::cout << "Input <" << input << "> rejected." << std::endl;
		}

	}while(input != exitToken);
}

bool proccessACase()
{
	std::cout << "RE (" << exitToken << " to exit): " << std::endl;

	std::string input;
	std::cin >> input;
	if(input == exitToken)
			return false;

	std::cout << "Input: " << input << std::endl;

	const auto postfixInput = ShuntingYard::SimpleAlgorithm::apply(input);
	std::cout << "Postfix Expression: " << postfixInput << std::endl;

	const auto nfa = Automata::Thompson::apply(postfixInput);
	std::cout << "Resulting NFA from Thompson's Construction: " << std::endl << nfa << std::endl;

	const auto dfa = Automata::Powerset::apply(nfa);
	std::cout << "Resulting DFA from Subset Construction: " << std::endl << dfa << std::endl;

	const auto minDfa = Automata::Hopcroft::apply(dfa);
	std::cout << "Resulting Minimal DFA from Hopcroft's Algorithm: " << std::endl << minDfa << std::endl;

	testDFA(minDfa);

	return true;
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	const bool runTests = false;

	printIntroduction();

	while(proccessACase());

	printConclution();

	if(runTests)
		return RUN_ALL_TESTS();
	else
		return EXIT_SUCCESS;
}
