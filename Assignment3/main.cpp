#include "utils.h"
#include "utilvars.h"


int main(int argc, char** argv) {

	if (argc < 2) {
		cerr << "Error: Missing filename! Use " << argv[0] << " <filename>" << std::endl;
		return -1;
	}

	parseInputFile(argv[1]);

	for (unsigned int i = 0; i < utils::netlist.size(); i++) {
		cout << "Net " << i + 1 << ": ";
		utils::nets[i].setSize(utils::netlist[i].size());
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
	long int x = 20 * (1 << utils::allBlocks.size());
//	cout << x << endl;

	const t_bound_box init_co = t_bound_box(0, 0,x , x+20);
	set_visible_world(init_co);

	event_loop(NULL, NULL, NULL, drawscreen);
	update_message("the tree");

	return 0;
}
