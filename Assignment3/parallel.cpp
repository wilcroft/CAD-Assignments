#include "parallel.h"


namespace utils {
	int bestCost = INT16_MAX;
}

#ifdef PARALLEL
std::mutex bestCostLock;
std::mutex nodeCountLock;

bool updateBestCost(int i) {
	bestCostLock.lock();

	if (utils::bestCost > i) {
		utils::bestCost = i;
		bestCostLock.unlock();
		return true;
	}
	bestCostLock.unlock();
	return true;
}
void addNodeCount() { 
	nodeCountLock.lock();
	utils::nodecount++; 
	nodeCountLock.unlock();
}
#else
bool updateBestCost(int i) {
	utils::bestCost = i;
	return true;
}
void addNodeCount() { utils::nodecount++; }
#endif
