#pragma once

#define DO_FULL_SPREADING 1

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <list>

#include <vector>
#include <algorithm>
#include <iterator>

#include <random>
#include <cfloat>

#include "graphics.h"
#include "block.h"
#include "net.h"

#include "SuiteSparse/UMFPACK/Include/umfpack.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace commonvars{
	//extern int graphn, graphw;
	//extern t_bound_box initial_coords;
	extern int maxNetNum;
	extern int numFreeBlocks;
	extern std::list<Block> allBlocks;
	extern std::list<Net> allNets;
	extern std::vector<std::list<Block *>> blocksAt;
	extern std::list<Block> tempRouting;
	void updateBlocksAt();
	std::list<Block *> getBlocksAt(int x, int y);
	std::list<Block *> getFreeBlocksAt(int x, int y);
}

//void popToFront(std::list<struct connections_t>* lst, std::list<struct connections_t>::iterator it);
//void popToFront(std::list<Connection>* lst, std::list<Connection>::iterator it);

int parseInputFile(char * fname);

void initialPlace(std::list<Block> * blocks);

void simpleOverlap();

void doRandomSwaps(std::mt19937 * mt);

int wireusage(std::list<Net> * nets);

void removeVirtualBlocks(std::list<Block> * blocks);
void removeFixedBlocks(std::list<Block *> * blocks);
void recurseRemoveOverlap(std::list<Block> * blocks, int i);

void drawscreen();




//std::list<Segment *> randomizeList(std::list<Segment *> l);

