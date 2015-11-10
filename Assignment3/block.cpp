#include "block.h"
#include "utilvars.h"

Block::Block() {
	side = NONE;
	bnum = -1;
}

Block::Block(int i) {
	side = NONE;
	bnum = i;
}

bool blockgreater(Block*  &a, Block* &b) {
	int asize = a->getConnections().size();
	int bsize = b->getConnections().size();
	if (asize > bsize) return true;
	else return false;
}

bool Block::operator<(const Block& b) {
	if (connections.size() < b.connections.size()) return true;
	else return false;
}
bool Block::operator>(const Block& b) {
	if (connections.size() > b.connections.size()) return true;
	else return false;
}
bool Block::operator==(const Block& b) {
	if (connections.size() == b.connections.size()) return true;
	else return false;
}

void Block::addNet(int i) {
	nets.push_back(i);
}

void Block::addConnection(int i) {
	connections.push_back(i);
}
void Block::printConnections() {
	for (auto& x : connections) {
		cout << x+1 << ", ";
	}
}
void Block::sortConnections() {
	std::sort(connections.begin(), connections.end());
	std::unique(connections.begin(), connections.end());
}

 std::vector<int>  Block::getConnections() { return connections; }

bool Block::isLeft() {
	return side==LEFTSIDE;
}
bool Block::isRight() {
	return side==RIGHTSIDE;
}
void Block::setLeft() {
	if (side != LEFTSIDE) {
		for (auto& x : nets) {
			if (side == RIGHTSIDE) utils::nets[x].removeRight(bnum);
			utils::nets[x].addLeft(bnum);
		}
		side = LEFTSIDE;
	}
}
void Block::setRight() {
	if (side != RIGHTSIDE) {
		for (auto& x : nets) {
			if (side == LEFTSIDE) utils::nets[x].removeLeft(bnum);
			utils::nets[x].addRight(bnum);
		}
		side = RIGHTSIDE;
	}
}
void Block::setNoSide() {
	for (auto& x : nets) utils::nets[x].remove(bnum);
	side = NONE;
}

void Block::setIndex(int i) { index = i; }
int Block::getIndex() { return index; }
void Block::setBnum(int i) { bnum = i; }
int Block::getBnum() { return bnum; }


int Block::cutCost(enum blockside b) {
	int cost = 0;
	for (auto &x :nets) {
		if (utils::nets[x].willCross(b)) cost++;
	}
	return cost;
}

bool Block::oppositeSide(Block &b) {
	if (side == LEFTSIDE && b.isRight()) return true;
	if (side == RIGHTSIDE && b.isLeft()) return true;
	return false;
}