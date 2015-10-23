#include "utils.h"


int main(int argc, char** argv) {

	int randomSwaps = 0;
	int seed = 0;

	if (argc < 2) {
		cerr << "Error: Missing filename! Use " << argv[0] << " <filename>" << std::endl;
		return -1;
	}

	if (argc > 3 && std::string(argv[2]) == "-swap") randomSwaps = atoi(argv[3]);
	if (argc > 4 && std::string(argv[2]) == "-swap") seed = atoi(argv[4]);
	else {
		std::random_device rd;
		seed = rd();
	}

	parseInputFile(argv[1]);
//	for (auto& x : commonvars::allBlocks)
//		x.print();

	cout << "Found " << commonvars::allBlocks.size() << " blocks." << endl;
	cout << "Found a max of " << commonvars::maxNetNum << " nets." << endl;

	init_graphics("Analytical Placer", WHITE);
	set_visible_world(0, 0, 1010, 1010);

	for (int i = 1; i <= commonvars::maxNetNum; i++) {
		commonvars::allNets.emplace_back(i);
		commonvars::allNets.back().buildBlockList(&commonvars::allBlocks);
//		commonvars::allNets.back().print();
	}

	//Part 2 - random IO swaps
	std::mt19937 mt(seed);
	for (int i = 0; i < randomSwaps; i++) {
		doRandomSwaps(&mt);
//		event_loop(NULL, NULL, NULL, drawscreen);
	}
	commonvars::tempRouting.clear();

	for (auto& x : commonvars::allNets) {
		x.buildConnections();// &commonvars::allBlocks);
	}

	initialPlace(&commonvars::allBlocks);
    cout << "Used "<< wireusage(&commonvars::allNets) << " units of wiring. " << endl;

	event_loop(NULL, NULL, NULL, drawscreen);

	simpleOverlap();
    cout << "O:Used "<< wireusage(&commonvars::allNets) << " units of wiring. " << endl;
 
	event_loop(NULL, NULL, NULL, drawscreen);

	return 0;
}
