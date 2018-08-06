#ifndef HOPCROFT_HOPCROFT_H_
#define HOPCROFT_HOPCROFT_H_

#include <Common.h>
#include <DFA.h>

namespace Automata {

class Hopcroft {
private:
	using IdType = int;
	struct Group
	{
		StateSetType states;
		IdType id;
		bool operator<(const Group& rhs) const { return id < rhs.id; }
		bool operator==(const Group& rhs) const { return states == rhs.states; }
		bool operator!=(const Group& rhs) const { return !operator==(rhs); }
	};
	using PartitionType = std::set<Group>;

public:
	Hopcroft() = delete;
	static DFA apply(const DFA&);

private:
	static PartitionType initialPartition(const DFA&);
	static PartitionType improvePartition(const DFA&, const PartitionType&, const AlphabetType&);
	static AlphabetType getAlphabet(const DFA&);
	static IdType findGroupId(const PartitionType&, const StateType&);
	static void printPartition(const PartitionType&);
	static void printGroup(const Group&);

};

} /* namespace Automata */

#endif /* HOPCROFT_HOPCROFT_H_ */
