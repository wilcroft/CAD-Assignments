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

void Net::setBlocks(std::list<int> * x) { blocks = x; }
std::list<int> * Net::getBlocks() { return blocks; }

int getCurrentCost(std::vector<Net> &n) {
	int cost = 0;
	for (auto&x : n) {
		if (x.isCrossing()) cost++;
	}
	return cost;
}

int getLBCost(std::vector<Net> &n, int lfree, int rfree, std::vector<Block> &b) {
	int cost = 0;
	std::vector<bool> netCosted;
	netCosted.resize(n.size());
	for (int i = 0; i < n.size(); i++) {
		if (n[i].isCrossing() || n[i].mustCross(lfree, LEFTSIDE) || n[i].mustCross(rfree, RIGHTSIDE)) {
			cost++;
			netCosted[i] = true;
		}
	}
	//for (auto&x : n) {
	//	if (x.isCrossing() || x.mustCross(lfree, LEFTSIDE) || x.mustCross(rfree, RIGHTSIDE)) cost++;
	//}

	for (auto&x : b) {
		if (x.isNoSide()) {
			std::list<int> nets = x.getNets();
			bool l = false, r = false;
			int i =0;
			for (auto&y : nets) {
				i++;
				if (!netCosted[y]) {
					for (auto& z : *n[y].getBlocks()) {
						if (!l &&b[z].isLeft()) l = true;
						if (!r &&b[z].isRight()) r = true;
						if (l&&r) break;
					}

				}
				if (l&&r) break;
			}
			if (l&&r) {
				cost++;
				for (auto&y : nets) {
					i--;
					netCosted[i] = true;
					if (i == 0) break;
				}
			}
		}
	}

	return cost;
}