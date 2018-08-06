#include "SimpleAlgorithm.h"

namespace ShuntingYard {

const SimpleAlgorithm::OperatorsDataType SimpleAlgorithm::operatorsData({{"*", 500, true}, {".", 400, true}, {"|", 300, true}});
const SimpleAlgorithm::TokenType SimpleAlgorithm::leftParenthesis("(");
const SimpleAlgorithm::TokenType SimpleAlgorithm::rightParenthesis(")");

std::string SimpleAlgorithm::apply(const std::string& input)
{
	ContainerType inputTokens;
	inputTokens.push_back(leftParenthesis);
	for(const auto& c: input)
		inputTokens.push_back(TokenType(1, c)); // TODO Find a better way to build a TokenType from char
	inputTokens.push_back(rightParenthesis);

	ContainerType outputTokens = run(inputTokens);

	std::ostringstream oss;
	for(const auto& token: outputTokens)
		oss << token;

	return oss.str();
}

SimpleAlgorithm::ContainerType SimpleAlgorithm::run(const ContainerType& tokens)
{
	OperatorStackType operators;
	OutputQueueType output;

	for(const auto& token: tokens)
	{
		if(leftParenthesis == token) processLeftParenthesis(operators);
		else if(rightParenthesis == token) processRightParenthesis(operators, output);
		else if(isOperator(token)) processOperator(token, operators, output);
		else output.push(token);
	}

	while(!operators.empty())
		output.push(operators.top()), operators.pop();

	ContainerType outputTokens;
	while(!output.empty())
		outputTokens.push_back(output.front()), output.pop();

	return outputTokens;
}

void SimpleAlgorithm::processLeftParenthesis(OperatorStackType& operators)
{
	operators.push(leftParenthesis);
}

void SimpleAlgorithm::processRightParenthesis(OperatorStackType& operators, OutputQueueType& output)
{
	while(operators.top() != leftParenthesis)
		output.push(operators.top()), operators.pop();
	operators.pop(); // pop leftParenthesis
}

void SimpleAlgorithm::processOperator(const TokenType& operatorToken, OperatorStackType& operators, OutputQueueType& output)
{
	TokenType topOperator = operators.top();
	while(leftParenthesis != topOperator &&
			( hasGreaterPrecedence(topOperator, operatorToken) || (hasEqualPrecedence(topOperator, operatorToken) && isLeftAssociative(topOperator)) )
	)
		output.push(operators.top()), operators.pop(), topOperator = operators.top();
	operators.push(operatorToken);
}

bool SimpleAlgorithm::isOperator(const TokenType& token)
{
	for(const auto& data: operatorsData)
		if(data.token == token)
			return true;
	return false;
}

bool SimpleAlgorithm::isLeftAssociative(const TokenType& operatorToken)
{
	for(const auto& data: operatorsData)
		if(data.token == operatorToken)
			return data.associativity;
	throw std::invalid_argument("Token does not correspond to a registered operator");
}

unsigned int SimpleAlgorithm::getPrecedence(const TokenType& operatorToken)
{
	for(const auto& data: operatorsData)
		if(data.token == operatorToken)
			return data.precedence;
	throw std::invalid_argument("Token does not correspond to a registered operator");
}

bool SimpleAlgorithm::hasGreaterPrecedence(const TokenType& operatorA, const TokenType& operatorB)
{
	return getPrecedence(operatorA) > getPrecedence(operatorB);
}

bool SimpleAlgorithm::hasEqualPrecedence(const TokenType& operatorA, const TokenType& operatorB)
{
	return getPrecedence(operatorA) == getPrecedence(operatorB);
}

} /* namespace ShuntingYard */
