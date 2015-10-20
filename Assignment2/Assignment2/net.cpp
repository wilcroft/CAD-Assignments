#include "net.h"

Net::Net() {
	idx = -1;
	real = true;
}

Net::Net(int i) {
	idx = i;
	real = true;
}
Net::Net(int i, bool r) {
	idx = i;
	real = r;
}

void Net::buildBlockList(std::list<Block> * blklst) {
	for (auto& x: *blklst)
		if (x.hasNet(idx)) blocks.emplace_back(&x);

}
void Net::print() {
	if (idx != -1 && blocks.begin() != blocks.end()) {
		std::list<Block *>::iterator it = blocks.begin();
		cout << "Net " << idx << " is connected to blocks (" << (*it)->getBlockNum();

		for (it++; it != blocks.end(); it++)
			cout << ", " << (*it)->getBlockNum();
		cout << ")" << endl;
	}
}
/* buildConnections
 * for the given net, connect all blocks using the clique model with weights 2/p
 */

void Net::buildConnections(){//std::list<Block> * blklst) {
	double w = 2.0 / blocks.size();

	//for (std::list<Block>::iterator ita = blklst->begin(); ita != blklst->end(); ita++) {
	for (auto& x:blocks){
		for (auto& y:blocks) {
			if (x != y) {
				x->addConnection(y, w);
			}
		}
	}
}

int Net::HPWL(){
	int xmin = 100;
	int ymin = 100;
	int xmax = 0;
	int ymax = 0;
	
	if (real) {
		for (auto& b : blocks) {
			if (b->getX() < xmin) xmin = b->getX();
			if (b->getX() > xmax) xmax = b->getX();
			if (b->getY() < ymin) ymin = b->getY();
			if (b->getY() > ymax) ymax = b->getY();
		}
		return xmax + ymax - ymin - xmin;
	}
	else return 0;
}

void Net::placeNet() {
	const int size = blocks.size();
	double weight = 2.0 / size;
	std::vector<int> xpos;
	std::vector<double> b;

	b.resize(size);
	int i = 0;
	for (auto& x : blocks) {
		if (x->isFixed()) {
			for (int j = 0; j < size; j++)
				if (j != i) b[j] += weight*x->getX();
		}
		i++;
	}


}
