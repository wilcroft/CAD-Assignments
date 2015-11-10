#pragma once
#include <list>
#include "block.h"

class Net {
private:
	std::list<int> left;
	std::list<int> right;

public:
//	Net();

	void clear();
	void clearLeft();
	void clearRight();

	void addLeft(int i);
	void removeLeft(int i);
	void addRight(int i);
	void removeRight(int i);
	void remove(int i);

	bool willCross(enum blockside b);

	bool isCrossing();


};