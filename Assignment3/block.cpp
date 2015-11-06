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

bool Block::blockgreater(const Block*  &a, const Block* &b) {
	if (a->connections.size() > b->connections.size()) return true;
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
}
bool Block::isLeft() {
	return side==LEFTSIDE;
}
bool Block::isRight() {
	return side==RIGHTSIDE;
}
void Block::setLeft() {
	side=LEFTSIDE;
}
void Block::setRight() {
	side=RIGHTSIDE;
}
void Block::setNoSide() {
	side = NONE;
}

void Block::setIndex(int i) { index = i; }
int Block::getIndex() { return index; }
void Block::setBnum(int i) { bnum = i; }
int Block::getBnum() { return bnum; }


int Block::cutCost(int b) {
	int count = 0;
	for (unsigned int i; i < connections.size(); i++) {
		if (connections[i] == b && oppositeSide(utils::allBlocks[b])) count++;
	}
	return count;
}
bool Block::oppositeSide(Block &b) {
	if (side == LEFTSIDE && b.isRight()) return true;
	if (side == RIGHTSIDE && b.isLeft()) return true;
	return false;
}