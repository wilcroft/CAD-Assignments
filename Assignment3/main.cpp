#include "utils.h"
#include "utilvars.h"


int main(int argc, char** argv) {

//	int randomSwaps = 0;
//	int seed = 0;

	if (argc < 2) {
		cerr << "Error: Missing filename! Use " << argv[0] << " <filename>" << std::endl;
		return -1;
	}

//	if (argc > 3 && std::string(argv[2]) == "-swap") randomSwaps = atoi(argv[3]);
//	if (argc > 4 && std::string(argv[2]) == "-swap") seed = atoi(argv[4]);
//	else {
//		std::random_device rd;
//		seed = rd();
//	}

	parseInputFile(argv[1]);

	for (unsigned int i = 0; i < utils::netlist.size(); i++) {
		cout << "Net " << i + 1 << ": ";
		for (auto &x : utils::netlist[i])
			cout << x + 1 << ", ";
		cout << endl;
	}

	for (unsigned int i = 0; i < utils::allBlocks.size(); i++) {
		cout << "Block " << i+1 << ": ";
		utils::allBlocks[i].sortConnections();
		utils::allBlocks[i].printConnections();
		cout << endl;
	}

	doBandB(utils::allBlocks);

	init_graphics("Branch+Bound", WHITE);
	int x = 20 * (1 << utils::allBlocks.size());
	cout << x << endl;

	const t_bound_box init_co = t_bound_box(0, 0,x , x+20);
	set_visible_world(init_co);

	event_loop(NULL, NULL, NULL, drawscreen);
	update_message("the tree");

	return 0;
}
