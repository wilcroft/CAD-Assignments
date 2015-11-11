#pragma once

#include "utilvars.h"

namespace utils {
	extern int bestCost;
}

#ifdef PARALLEL
#include <thread>
#include <mutex>

#endif



bool updateBestCost(int i);
void addNodeCount();




