#include "net.h"


void Net::clear() {
	left.clear();
	right.clear();
	count = left.size() + right.size();
}
void Net::clearLeft() {
	left.clear();
	count = left.size() + right.size();
}
void Net::clearRight() { 
	right.clear();
	count = left.size() + right.size();
}

void Net::setSize(int i) { size = i; }
int Net::getSize() { return size; }

void Net::addLeft(int i) { 
	right.remove(i);
	left.push_back(i);
	left.unique();
	left.sort();
	count = left.size() + right.size();
}

void Net::removeLeft(int i){
	left.remove(i);
	count = left.size() + right.size();
}
void Net::addRight(int i){
	left.remove(i);
	right.push_back(i);
	right.unique();
	right.sort();
	count = left.size() + right.size();
}
void Net::removeRight(int i) {
	right.remove(i);
	count = left.size() + right.size();
}
void Net::remove(int i) {
	left.remove(i);
	right.remove(i);
	count = left.size() + right.size();
}

bool Net::willCross(enum blockside b) {
	if (left.size() > 0 && right.size() == 0 && b == RIGHTSIDE) return true;
	if (left.size() == 0 && right.size() > 0 && b == LEFTSIDE) return true;
	return false;
}

bool Net::mustCross(int spaceToPlace, enum blockside b) {
	if (b == LEFTSIDE && right.size() == 0 && spaceToPlace < size - count) return true;
	if (b == RIGHTSIDE && left.size() == 0 && spaceToPlace < size - count) return true;
	return false;
}

bool Net::isCrossing() {
	if (left.size() > 0 && right.size() > 0) return true;
	else return false;
}

int getCurrentCost(std::vector<Net> &n) {
	int cost = 0;
	for (auto&x : n) {
		if (x.isCrossing()) cost++;
	}
	return cost;
}
int getLBCost(std::vector<Net> &n, int lfree, int rfree) {
	int cost = 0;
	for (auto&x : n) {
		if (x.isCrossing() || x.mustCross(lfree, LEFTSIDE) || x.mustCross(rfree, RIGHTSIDE)) cost++;
	}
	return cost;
}