#include "block.h"

Block::Block() {
	num = -1;
	x = -1;
	y = -1;
	fixed = false;
}
Block::Block(int i) {
	num = i;
	x = -1;
	y = -1;
	fixed = false;

}
Block::Block(int i, std::list<int> l) {
	num = i;
	x = -1;
	y = -1;
	nets = l;
	fixed = false;

}
Block::Block(int i, int xx, int yy) {
	num = i;
	x = xx;
	y = yy;
	fixed = false;

}
Block::Block(int i, int xx, int yy, std::list<int> l) {
	num = i;
	x = xx;
	y = yy;
	nets = l;
	fixed = false;

}

bool Block::hasNet(int i) {
	for (std::list<int>::iterator it = nets.begin(); it != nets.end(); it++)
		if (*it == i) return true;
	return false;
}

void Block::addNet(int i) {
	nets.push_back(i);
}

int Block::getBlockNum() { return num; }

void Block::setX(int xx) { x = xx; }
void Block::setY(int yy) { y = yy; }
int Block::getX() { return x; }
int Block::getY() { return y; }
void Block::setFixed(bool f) {
	fixed = f;
}
bool Block::isFixed() { return fixed; }

void Block::addConnection(std::pair<Block *, double> c) {
	connections.push_back(c);
}
void Block::addConnection(Block * b, double w) {
	connections.emplace_back(b, w);
}

double Block::getSumWeights() {
	double sum = 0;
	for (auto& x:connections){
		sum += x.second;
	}
	return sum;
}

double Block::getWeight(Block * b) {
	double sum = 0;
	for (auto& x : connections) {
		if (x.first==b)
			sum += x.second;
	}
	return sum;
}

void Block::print() {
	if (num != -1) {
		std::list<int>::iterator it = nets.begin();
		cout << "Block " << num << " is connected to nets (" << *it;

		for (it++; it != nets.end(); it++)
			cout << ", " << *it;
		cout << ")" << endl;
	}
}