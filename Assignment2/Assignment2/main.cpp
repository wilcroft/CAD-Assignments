#include "utils.h"


int main(int argc, char** argv) {
	if (argc < 2) {
		cerr << "Error: Missing filename! Use " << argv[0] << " <filename>" << std::endl;
		return -1;
	}

	parseInputFile(argv[1]);
	for (auto& x : commonvars::allBlocks)
		x.print();

	cout << "Found a max of " << commonvars::maxNetNum << " nets." << endl;

	for (int i = 1; i <= commonvars::maxNetNum; i++) {
		commonvars::allNets.emplace_back(i);
		commonvars::allNets.rbegin()->buildBlockList(&commonvars::allBlocks);
		commonvars::allNets.rbegin()->print();
	}

	for (auto& x : commonvars::allNets) {
		x.buildConnections();// &commonvars::allBlocks);
	}

	initialPlace(&commonvars::allBlocks);
    cout << "Used "<< wireusage(&commonvars::allNets) << " units of wiring. " << endl;

	return 0;
}
