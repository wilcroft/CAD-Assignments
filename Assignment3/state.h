#pragma once

#include <vector>

#include "block.h"
#include "net.h"


class State {
public:
	std::vector<Block> blocks;
	std::vector<Net> nets;

	int cost;
	int maxcount;
	int lcount;
	int rcount;
	int depth;

	std::list<Block*> * queue;
	std::list<Block*>::iterator it;
	Tree * treenode;

};

class statecmp {
public:
	bool operator() (const State& lhs, const State& rhs) {
		if (lhs.depth == rhs.depth) 
			return (lhs.cost > rhs.cost);
		else return (lhs.depth < rhs.depth);
	}
};