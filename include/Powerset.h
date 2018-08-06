#ifndef POWERSET_H_
#define POWERSET_H_

#include <map>
#include <queue>
#include <NFA.h>
#include <DFA.h>

namespace Automata {

class Powerset
{
public:
	Powerset() = delete;
	static DFA apply(const NFA&);
private:
	static StateSetType moveOverSet(const NFA&, const StateSetType&, const SymbolType&);
	static AlphabetType getAlphabet(const NFA&);
	static StateSetType multipleMove(const NFA&, const StateSetType&, const SymbolType&);
	static bool containsAFinalState(const StateSetType&, const StateSetType&);
};

} /* namespace Automata */

#endif /* POWERSET_H_ */
