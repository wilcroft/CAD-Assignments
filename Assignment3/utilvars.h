#pragma once
#include <vector>
#include <list>
#include <map>
#include "block.h"
#include "tree.h"
#include "net.h"

namespace utils {
	extern std::vector<Block> allBlocks;
	extern std::vector<std::list<int>> netlist;
	//extern std::map<std::pair<Block*, enum blockside>, std::pair<int, bool>> tree;
	extern Tree * bbTree;
	extern std::vector<Net> nets;
	extern unsigned long int nodecount;
}
