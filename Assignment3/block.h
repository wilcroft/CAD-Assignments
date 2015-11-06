#pragma once

#include <list>
#include <vector>
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

enum blockside {
	NONE,
	LEFTSIDE,
	RIGHTSIDE
};


class Block {
private:
	//std::list<int> nets;
	std::vector<int> connections;
	enum blockside side;
	int index;
	int bnum;

public:
	Block();
	Block(int i);
	bool operator< (const Block&);
	bool operator> (const Block&);
	bool operator== (const Block&);

	void addConnection(int i);
	void printConnections();
	void sortConnections();
	bool isLeft();
	bool isRight();
	void setLeft();
	void setRight();
	void setNoSide();

	void setIndex(int i);
	int getIndex();
	void setBnum(int i);
	int getBnum();

	int cutCost(int b);
	bool oppositeSide(Block &b);

	bool blockgreater(const Block*  &a, const Block* &b);
};