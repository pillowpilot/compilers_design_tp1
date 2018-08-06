#ifndef SHUNTINGYARD_SIMPLEALGORITHM_H_
#define SHUNTINGYARD_SIMPLEALGORITHM_H_
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <queue>
#include <iostream>

namespace ShuntingYard {

class SimpleAlgorithm {
public:
	static std::string apply(const std::string&);

protected:
	using TokenType = std::string;
	struct OperatorData
	{
		const TokenType token;
		const unsigned int precedence;
		const bool associativity;
	};
	using ContainerType = std::vector<TokenType>;
	using OperatorsDataType = std::vector<OperatorData>;
	using OperatorStackType = std::stack<TokenType>;
	using OutputQueueType = std::queue<TokenType>;

	static const OperatorsDataType operatorsData;
	static const TokenType leftParenthesis;
	static const TokenType rightParenthesis;

	static ContainerType run(const ContainerType&);
	static void processLeftParenthesis(OperatorStackType&);
	static void processRightParenthesis(OperatorStackType&, OutputQueueType&);
	static void processOperator(const TokenType&, OperatorStackType&, OutputQueueType&);
	static bool isOperator(const TokenType&);
	static bool isLeftAssociative(const TokenType&);
	static unsigned int getPrecedence(const TokenType&);
	static bool hasGreaterPrecedence(const TokenType&, const TokenType&);
	static bool hasEqualPrecedence(const TokenType&, const TokenType&);
};

} /* namespace ShuntingYard */

#endif /* SHUNTINGYARD_SIMPLEALGORITHM_H_ */
