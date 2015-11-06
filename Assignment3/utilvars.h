#pragma once
#include <vector>
#include <list>
#include <map>
#include "block.h"

namespace utils {
	extern std::vector<Block> allBlocks;
	extern std::vector<std::list<int>> netlist;
	extern std::map<std::pair<Block*, enum blockside>, std::pair<int, bool>> tree;
}