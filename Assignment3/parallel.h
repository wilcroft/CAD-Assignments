#pragma once

#include "utilvars.h"
namespace utils {
	extern int bestCost;
}

#ifdef PARALLEL
#include <thread>
#include <mutex>
#include <future>
#define THREADCOUNT 16

namespace thds {
	extern std::thread * workers[THREADCOUNT];
	extern bool workeractive[THREADCOUNT];
	extern int active;
	extern std::mutex thdLock;
	extern std::mutex heapLock;
}
#endif



bool updateBestCost(int i);
void addNodeCount();




