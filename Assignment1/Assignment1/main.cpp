#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <list>

#define DRAWEACHROUTE 0
#define MAXATTEMPTS 100

#include "utils.h"
#include "connection.h"
#include "graphics.h"


using std::string;
using std::cout;
using std::cerr;
using std::endl;



//extern t_bound_box initial_coords;
//extern Channel * routing;

int main(int argc, char ** argv){
	int chipn, chipw;
	int attempts = 0;
	std::list<Connection> connlist;
	//std::list<pin> sourcelist;

	if (argc < 2) {
		cerr << "Error: Missing filename! Use " << argv[0] << " <filename>" << std::endl;
		return -1;
	}

	if (parseInputFile(argv[1], &chipn, &chipw, &connlist) != 0) return -1;
	//printConnList(connlist);

	utilvars::initial_coords = t_bound_box(0, 0, (chipw * 2 + 1)*(2 * chipn + 1), (chipw * 2 + 1)*(2 * chipn + 1));

	init_graphics("Maze Routing", WHITE);
	set_visible_world(utilvars::initial_coords);

	std::list<Connection>::iterator iter = connlist.begin();
	utilvars::routing = new Channel(chipn, chipw);

	int cindex = YELLOW + 1;
	for (iter; iter != connlist.end(); iter++){
		//sourcelist.push_back(iter->src());
		utilvars::colormap.emplace(iter->src(), (color_types)cindex);
		cindex++;
		if (cindex%NUM_COLOR==0) cindex = YELLOW + 1;
	}


	iter = connlist.begin();
	while (iter != connlist.end() && attempts<MAXATTEMPTS){
		if (DRAWEACHROUTE)
			event_loop(NULL, NULL, NULL, drawscreen);

		cout << "Attempting Route: ";
		iter->print();

		//bool bar = utilvars::routing->route(iter->src(), iter->dest());

		if (!utilvars::routing->route(iter->src(), iter->dest())){
			attempts++;
			string message = "Resetting routing - Attempt #" + std::to_string(attempts + 1) + "...";
			update_message(message);
			event_loop(NULL, NULL, NULL, drawscreen);
			popToFront(&connlist, iter);
			iter = connlist.begin();
			utilvars::routing->resetRouting();
		}
		else{
			utilvars::routing->clearAttempt();
			iter++;
			update_message("...");
		}
	}
	if (attempts == MAXATTEMPTS){
		string message = "Could not route after " + std::to_string(MAXATTEMPTS) + " attempts. Giving up.";
		update_message(message);
	}
	else	update_message("Done!");
	cout << "Used " << utilvars::routing->routingSegmentsUsed() << " wire segments." << endl;
	cout << "Widest channel used: " << utilvars::routing->maxW() << endl;
	event_loop(NULL, NULL, NULL, drawscreen);
	return 0;
}