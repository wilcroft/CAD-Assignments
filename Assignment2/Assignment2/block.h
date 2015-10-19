#ifndef __A2_BLOCK_H__
#define __A2_BLOCK_H__

#include <list>
#include <iostream>

using std::cout;
using std::endl;

class Block {
private:
	int num;
	std::list<int> nets;
	std::list<std::pair<Block *, float>> connections;

	int x;
	int y;
	bool fixed;

public:
	Block();
	Block(int i);
	Block(int i, std::list<int> l);
	Block(int i, int xx, int yy);
	Block(int i, int xx, int yy, std::list<int> l);
	//~Block();

	bool hasNet(int i);
	void addNet(int i);
	void setX(int xx);
	void setY(int yy);
	int getX();
	int getY();
	int getBlockNum();
	void setFixed(bool f=true);
	bool isFixed();
	void addConnection(std::pair<Block *, float> c);
	void addConnection(Block * b, float w);
	float getSumWeights();
	float getWeight(Block * b);
	void print();

};



#endif
