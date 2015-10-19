#pragma once

#include <list>
#include <vector>
#include <iostream>
#include "block.h"

using std::cout;
using std::endl;


class Net {
private:
	std::list<Block *> blocks;
	int idx;
	int x;
	int y;


public:
	Net();
	Net(int i);

	void buildBlockList(std::list<Block> * blklst);
	void buildConnections();// std::list<Block> * blklst);
	int HPWL ();
	void print();
	void placeNet();
};
