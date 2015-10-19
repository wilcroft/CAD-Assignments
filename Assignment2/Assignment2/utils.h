#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <list>

#include <vector>
#include <algorithm>
#include <map>

#include "graphics.h"
#include "block.h"
#include "net.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace commonvars{
	extern int graphn, graphw;
	extern t_bound_box initial_coords;
	extern int maxNetNum;
	extern int numFreeBlocks;
	extern std::list<Block> allBlocks;
	extern std::list<Net> allNets;
}

//void popToFront(std::list<struct connections_t>* lst, std::list<struct connections_t>::iterator it);
//void popToFront(std::list<Connection>* lst, std::list<Connection>::iterator it);

int parseInputFile(char * fname);

void initialPlace(std::list<Block> * blocks);

void drawscreen();
//std::list<Segment *> randomizeList(std::list<Segment *> l);

