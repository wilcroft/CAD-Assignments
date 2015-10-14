#include "block.h"

Block::Block() {
	num = -1;
	x = -1;
	y = -1;
}
Block::Block(int i) {
	num = i;
	x = -1;
	y = -1;

}
Block::Block(int i, std::list<int> l) {
	num = i;
	x = -1;
	y = -1;
	nets = l;

}
Block::Block(int i, int xx, int yy) {
	num = i;
	x = xx;
	y = yy;

}
Block::Block(int i, int xx, int yy, std::list<int> l) {
	num = i;
	x = xx;
	y = yy;
	nets = l;

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

void Block::print() {
	if (num != -1) {
		std::list<int>::iterator it = nets.begin();
		cout << "Block " << num << " is connected to nets (" << *it;

		for (it++; it != nets.end(); it++)
			cout << ", " << *it;
		cout << ")" << endl;
	}
}