#include "net.h"


void Net::clear() {
	left.clear();
	right.clear();
}
void Net::clearLeft() { left.clear(); }
void Net::clearRight() { right.clear(); }

void Net::addLeft(int i) { 
	left.push_back(i);
	left.unique();
	left.sort();
}

void Net::removeLeft(int i){
	left.remove(i);
}
void Net::addRight(int i){
	right.push_back(i);
	right.unique();
	right.sort();
}
void Net::removeRight(int i) {
	right.remove(i);
}
void Net::remove(int i) {
	left.remove(i);
	right.remove(i);
}

bool Net::willCross(enum blockside b) {
	if (left.size() > 0 && right.size() == 0 && b == RIGHTSIDE) return true;
	if (left.size() == 0 && right.size() > 0 && b == LEFTSIDE) return true;
	return false;
}

bool Net::isCrossing() {
	if (left.size() > 0 && right.size() > 0) return true;
	else return false;
}