#pragma once

#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include "utilvars.h"

using std::cout;
using std::endl;

enum blockside {
	NONE,
	LEFTSIDE,
	RIGHTSIDE
};

class Net;

class Block {
private:
	std::list<int> nets;
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

	void addNet(int i);

	void addConnection(int i);
	void printConnections();
	void sortConnections();
	 std::vector<int>  getConnections();
	bool isLeft();
	bool isRight();
	void setLeft(std::vector<Net> & netv = utils::nets);
	void setRight(std::vector<Net> & netv = utils::nets);
	void setNoSide(std::vector<Net> & netv = utils::nets);

	void setIndex(int i);
	int getIndex();
	void setBnum(int i);
	int getBnum();

	int cutCost(enum blockside b);
	int cutCost(enum blockside b, std::vector<Net> &netv);
	bool oppositeSide(Block &b);

	
};
bool blockgreater( Block*  &a,  Block* &b);
void printBlockVector(std::vector<Block> &b);