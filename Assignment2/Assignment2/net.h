#pragma once

#include <list>
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
	void print();
};