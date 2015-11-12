#include "utils.h"
#include "utilvars.h"
#include <ctime>


int main(int argc, char** argv) {
	uint64_t t0, t1, t2, t3;

	t0 = clock();
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

	t1 = clock();

	doBandB(utils::allBlocks);
	//doHeapedBandB(utils::allBlocks);

	t2 = clock();

	init_graphics("Branch+Bound", WHITE);
	float y = 20.0f * (1 << utils::allBlocks.size());
	uint64_t x = 100 * (1 << utils::allBlocks.size());
//	cout << x << endl;

	const t_bound_box init_co = t_bound_box(0, 0, x, y+20);
	set_visible_world(init_co);

	event_loop(NULL, NULL, NULL, drawscreen);
	update_message("the tree");
	t3 = clock();

	cout << "Times:" << endl;
	cout << "Initialization: " << ((double)(t1 - t0)) * 1000 / CLOCKS_PER_SEC << "ms" << endl;
	cout << "Branch+Bound:   " << ((double)(t2 - t1)) * 1000 / CLOCKS_PER_SEC << "ms" << endl;
	cout << "Graphics:       " << ((double)(t3 - t2)) * 1000 / CLOCKS_PER_SEC << "ms" << endl;
	cout << "Total:          " << ((double)(t3 - t0)) * 1000 / CLOCKS_PER_SEC << "ms" << endl;


	delete utils::bbTree;

	return 0;
}
