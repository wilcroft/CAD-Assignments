#pragma once
#include <list>
#include "block.h"

class Net {
private:
	std::list<int> left;
	std::list<int> right;
	int count;  // number of blocks on left and right
	int size; //number of blocks in the net

public:
//	Net();

	void clear();
	void clearLeft();
	void clearRight();
	void setSize(int i);
	int getSize();

	void addLeft(int i);
	void removeLeft(int i);
	void addRight(int i);
	void removeRight(int i);
	void remove(int i);

	bool willCross(enum blockside b);
	bool mustCross(int spaceToPlace, enum blockside b);
	bool isCrossing();


};

int getCurrentCost(std::vector<Net>& n);
int getLBCost(std::vector<Net> &n, int lfree, int rfree);