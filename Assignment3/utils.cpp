#include "utils.h"
#include "utilvars.h"
#include "parallel.h"

namespace utils {
	std::vector<Block> allBlocks;
	std::vector<std::list<int>> netlist;
	Tree * bbTree;
	std::vector<Net> nets;
	unsigned long int nodecount = 0;
}

int parseInputFile(char * fname) {
	std::ifstream fs(fname);
	string temp;
	int blocknum;
	int tempint;
	std::vector<std::list<Block *>> nets;


	size_t idx = 0;

	if (fs.fail()) {
		cerr << "Error: Couldn't open file \"" << fname << "\"" << endl;
		return -1;
	}

	//Read the Blocks + Nets
	do {
		std::getline(fs, temp);
		if (fs.eof()) {
			cerr << "Error: block list in file \"" << fname << "\" should be terminated with -1" << endl;
			return -1;
		}
		//Get block number
		try { blocknum = std::stoi(temp, &idx); } //ensure conversion from string to int
		catch (const std::invalid_argument& ia) {
			cerr << "Error: '" << temp << "' is not a valid decimal integer" << endl;
			return -1;
		}
		temp.erase(0, idx);
		if (blocknum != -1) {
			if (blocknum > (signed int)utils::allBlocks.size()) utils::allBlocks.resize(blocknum);
			do {
				//Get nets for the block number
				try { tempint = std::stoi(temp, &idx); } //ensure conversion from string to int
				catch (const std::invalid_argument& ia) {
					cerr << "Error: '" << temp << "' is not a valid decimal integer" << endl;
					return -1;
				}
				temp.erase(0, idx);
				if (tempint > (signed int)utils::nets.size())
					utils::nets.resize(tempint);
				if (tempint != -1)
					utils::allBlocks[blocknum - 1].addNet(tempint - 1);
				if (tempint > (signed int)utils::netlist.size())
					utils::netlist.resize(tempint);
				if (tempint != -1)
					utils::netlist[tempint - 1].push_back(blocknum - 1);
			} while (tempint != -1);
		}
	} while (blocknum != -1);

	for (auto &x : utils::netlist) {
		if (x.size() > 1) {
			for (auto &i : x) {
				for (auto&j : x) {
					if (i != j) utils::allBlocks[i].addConnection(j);
				}
			}
		}
	}

	for (unsigned int i = 0; i < utils::allBlocks.size(); i++) {
		utils::allBlocks[i].setBnum(i);
	}

	fs.close();

	return 0;
}

std::list <Block *> getOrderedList(std::vector<Block> &b) {
	std::list<Block *> bp;
	for (auto&x : b) {
		bp.push_back(&x);
	}

	bp.sort(blockgreater);

	int i = 0;
	for (auto&x : bp) {
		x->setIndex(i++);
	}

	return bp;
}

void doBandB(std::vector<Block> &blocks) {
	std::list<Block *> queue = getOrderedList(blocks);
	unsigned int count = (unsigned int)ceil(blocks.size()/2.0);
	unsigned int lcount = 0, rcount = 0;

	int bestCost;
	int netBestCost;
	int randBestCost;
	
	//Get inital "best cost"
	bestCost = initialCost(*queue.begin(), count);
	cout << "Best Cost: " << bestCost << endl;
	netBestCost = initialCostNet(count);
	cout << "Net Best Cost: " << netBestCost << endl;
	randBestCost = initialCostRandom(count);
	cout << "Rand Best Cost: " << randBestCost << endl;
	if (netBestCost > bestCost && randBestCost > bestCost) cout << "Using 'Best Cost'" << endl;
	else if (randBestCost > netBestCost) {
		bestCost = netBestCost;
		cout << "Using 'Net Best Cost'" << endl;
	}
	else {
		bestCost = randBestCost;
		cout << "Using 'Rand Best Cost'" << endl;
	}

	utils::bbTree = new Tree();
	std::list<Block*>::iterator it = queue.begin();
	(*it)->setLeft();
	lcount++;
	it++;
	
	State s;
	s.blocks = utils::allBlocks;
	s.nets = utils::nets;
	s.maxcount = count;
	s.lcount = 1;
	s.rcount = 0;

	//cout << "Old:" << endl;
	//exploreTree(queue, it, 0, bestCost, lcount, rcount, count, utils::bbTree);
	//cout << "New:" << endl;
	//it = queue.begin();
	//it++;
	updateBestCost(bestCost);
	exploreTree(queue, it, s, utils::bbTree);
	bestCost = utils::bestCost;

	
	cout << "Best Cost: " << bestCost << endl;
	cout << "Node Count " << utils::nodecount << endl;

}

void exploreTree(std::list<Block*> &blocks, std::list<Block*>::iterator it, int currCost, int& bestCost, int lcount, int rcount, const int maxcount, Tree * treenode) {
	int newcost;
	int lbcost;

	if (lcount > maxcount)
		return;
	if (rcount > maxcount) 
		return;
	lbcost = getLBCost(utils::nets, maxcount - lcount, maxcount - rcount);
	if (lbcost > bestCost) return;
	if (it == blocks.end() && currCost < bestCost) {
		cout << "(" << currCost << ") ";
		printBlockVector(utils::allBlocks);
		bestCost = currCost;
		return;
	}
	Block * bcurr = *it;
	newcost = currCost + (bcurr)->cutCost(LEFTSIDE);
	if (newcost<bestCost) {
		(bcurr)->setLeft();
//		cout << "Block " << bcurr->getBnum() + 1 << " Left: Current Cost = " << newcost << endl;
		it++;
		utils::nodecount++;
		exploreTree(blocks, it, newcost, bestCost, lcount + 1, rcount, maxcount, treenode->addLeft(newcost));
		it--;
		(bcurr)->setNoSide();
	}
	newcost = currCost + (bcurr)->cutCost(RIGHTSIDE);
	if (newcost<bestCost) {
		(bcurr)->setRight();
//		cout << "Block " << bcurr->getBnum() + 1 << " Right: Current Cost = " << newcost << endl;
		it++;
		utils::nodecount++;
		exploreTree(blocks, it, newcost, bestCost, lcount, rcount+1, maxcount, treenode->addRight(newcost));
		it--;
		(bcurr)->setNoSide();
	}
	
	return;

}

void exploreTree(std::list<Block*> &blocks, std::list<Block*>::iterator it, State &s, Tree * treenode) {
	int newcost;
	int lbcost;
	s.cost = getCurrentCost(s.nets);

	if (s.lcount > s.maxcount)
		return;
	if (s.rcount > s.maxcount)
		return;
	lbcost = getLBCost(s.nets, s.maxcount - s.lcount, s.maxcount - s.rcount);
	if (lbcost > utils::bestCost) return;
	if (it == blocks.end() && s.cost < utils::bestCost) {
		if (updateBestCost(s.cost))
			cout << "(" << s.cost << ") ";
		printBlockVector(s.blocks);
		return;
	}
	int idx = (*it)->getBnum();
	newcost = s.cost + s.blocks[idx].cutCost(LEFTSIDE,s.nets);
	if (newcost<utils::bestCost) {
		State sleft = s;
		sleft.blocks[idx].setLeft(sleft.nets);
		sleft.lcount++;
		//		cout << "Block " << bcurr->getBnum() + 1 << " Left: Current Cost = " << newcost << endl;
		it++;
		addNodeCount();
		exploreTree(blocks, it, sleft, treenode->addLeft(newcost));
		it--;
	}
	newcost = s.cost + s.blocks[idx].cutCost(RIGHTSIDE, s.nets);
	if (newcost<utils::bestCost) {
		State sright = s;
		sright.blocks[idx].setRight(sright.nets);
		sright.rcount++;
		//		cout << "Block " << bcurr->getBnum() + 1 << " Right: Current Cost = " << newcost << endl;
		it++;
		addNodeCount();
		exploreTree(blocks, it, sright, treenode->addRight(newcost));
		it--;
	}

	return;

}

void doHeapedBandB(std::vector<Block> &blocks) {

	std::list<Block *> queue = getOrderedList(blocks);
	unsigned int count = (unsigned int)ceil(blocks.size() / 2.0);
	unsigned int lcount = 0, rcount = 0;

	int bestCost;
	int netBestCost;
	int randBestCost;

	//Get inital "best cost"
	bestCost = initialCost(*queue.begin(), count);
	cout << "Best Cost: " << bestCost << endl;
	netBestCost = initialCostNet(count);
	cout << "Net Best Cost: " << netBestCost << endl;
	randBestCost = initialCostRandom(count);
	cout << "Rand Best Cost: " << randBestCost << endl;
	if (netBestCost > bestCost && randBestCost > bestCost) cout << "Using 'Best Cost'" << endl;
	else if (randBestCost > netBestCost) {
		bestCost = netBestCost;
		cout << "Using 'Net Best Cost'" << endl;
	}
	else {
		bestCost = randBestCost;
		cout << "Using 'Rand Best Cost'" << endl;
	}
	updateBestCost(bestCost);

	utils::bbTree = new Tree();
	std::list<Block*>::iterator it = queue.begin();
	(*it)->setLeft();
	lcount++;
	it++;

	State s;
	s.blocks = utils::allBlocks;
	s.nets = utils::nets;
	s.maxcount = count;
	s.lcount = 1;
	s.rcount = 0;

	std::priority_queue<State, std::vector<State>, statecmp> stateQueue;
	stateQueue.push(s);

	while (stateQueue.size() != 0) {
		s = stateQueue.top();
		stateQueue.pop();
		exploreState(s,&stateQueue);
	}

	cout << "Best Cost: " << utils::bestCost << endl;
	cout << "Node Count " << utils::nodecount << endl;

}

void exploreState(State &s, std::priority_queue<State, std::vector<State>, statecmp> * stateQueue) {
	int newcost;
	int lbcost;
	s.cost = getCurrentCost(s.nets);

	if (s.lcount > s.maxcount)
		return;
	if (s.rcount > s.maxcount)
		return;
	lbcost = getLBCost(s.nets, s.maxcount - s.lcount, s.maxcount - s.rcount);
	if (lbcost > utils::bestCost) return;
	if (s.it == s.queue->end() && s.cost < utils::bestCost) {
		if (updateBestCost(s.cost))
			cout << "(" << s.cost << ") ";
		printBlockVector(s.blocks);
		return;
	}
	int idx = (*s.it)->getBnum();
	newcost = s.cost + s.blocks[idx].cutCost(LEFTSIDE, s.nets);
	if (newcost<utils::bestCost) {
		State sleft = s;
		sleft.blocks[idx].setLeft(sleft.nets);
		sleft.lcount++;
		s.it++;
		sleft.it = s.it;
		sleft.treenode = s.treenode->addLeft(newcost);
		addNodeCount();
		stateQueue->push(sleft);
		s.it--;
	}
	newcost = s.cost + s.blocks[idx].cutCost(RIGHTSIDE, s.nets);
	if (newcost<utils::bestCost) {
		State sright = s;
		sright.blocks[idx].setRight(sright.nets);
		sright.rcount++;
		s.it++;
		sright.it = s.it;
		sright.treenode = s.treenode->addRight(newcost);
		addNodeCount();
		stateQueue->push(sright);
		s.it--;
	}

	return;

}


int initialCost(Block * b, int maxcount) {
	std::vector<int> blocks;
	std::vector<int> temp;
	int i = 0;
	int cost = 0;

	blocks.push_back(b->getBnum());
	b->setLeft();
	temp = b->getConnections();
	blocks.insert(blocks.end(), temp.begin(), temp.end());
	i++;
	while (i < maxcount) {
		if (utils::allBlocks[blocks[i]].isLeft()) {
			blocks.erase(blocks.begin() + i);
		}
		else {
			temp.clear();
			utils::allBlocks[blocks[i]].setLeft();
			temp = utils::allBlocks[blocks[i]].getConnections();
			blocks.insert(blocks.end(), temp.begin(), temp.end());
			i++;
		}
	}
	for (auto& x : utils::allBlocks) {
		if (!x.isLeft()) x.setRight();
	}

	for (auto& x : utils::nets) {
		if (x.isCrossing()) cost++;
	}

	for (auto&x:utils::allBlocks) {
		x.setNoSide();
	}

	return cost;

}
bool sortNetPtrs(std::list<int>* a, std::list<int> * b) {
	if (a->size() > b->size()) return true;
	else return false;
}

int initialCostNet(int maxcount) {
	std::list<std::list<int>*> sortedNets;
	for (unsigned int i = 0; i < utils::netlist.size(); i++) {
		sortedNets.push_back(&utils::netlist[i]);
	}
	//for (auto x : utils::netlist) {
//		sortedNets.push_back(&x);
	//}
	sortedNets.sort(sortNetPtrs);
	std::list<std::list<int>*>::iterator netit;
	std::list<int>::iterator blockit;

	int blockcount = 0;
	int cost = 0;

	for (netit = sortedNets.begin(); blockcount < maxcount; netit++) {
		for (blockit = (*netit)->begin(); blockcount < maxcount && blockit != (*netit)->end(); blockit++) {
			if (!utils::allBlocks[*blockit].isLeft()) {
				blockcount++;
				utils::allBlocks[*blockit].setLeft();
			}
		}
	}
	for (auto&x : utils::allBlocks) {
		if (!x.isLeft()) x.setRight();
	}

	for (auto& x : utils::nets) {
		if (x.isCrossing()) cost++;
	}

	for (auto&x : utils::allBlocks) {
		x.setNoSide();
	}

	return cost;
}

int initialCostRandom(int maxcount) {
	std::vector<Block> randBlocks = utils::allBlocks;
	Block b;
	int cost = 0;

	for (int i = 0; i < maxcount; i++) {
		std::random_shuffle(randBlocks.begin(), randBlocks.end());
		b = randBlocks.back();
		randBlocks.pop_back();
		utils::allBlocks[b.getBnum()].setLeft();
	}
	for (auto&x : utils::allBlocks) {
		if (!x.isLeft()) x.setRight();
	}

	for (auto& x : utils::nets) {
		if (x.isCrossing()) cost++;
	}

	for (auto&x : utils::allBlocks) {
		x.setNoSide();
	}

	return cost;

}


void drawscreen() {
    set_draw_mode(DRAW_NORMAL);
    clearscreen();
    setlinestyle(SOLID);
    setlinewidth(1);

	float x = 10.0f * (1 << utils::allBlocks.size());
	float y = 20.0f * (1 << utils::allBlocks.size()) -5;
	float dy = 20.0f * (1 << utils::allBlocks.size()) / utils::allBlocks.size();

    setfontsize(12);

	setfontsize(12);
	cout << "Font Size: " << getfontsize() << endl;

	Tree * ptr = utils::bbTree;
	drawTree(ptr, x, y, utils::allBlocks.size()-1,dy);

}

void drawTree(Tree * ptr, float x, float y, int i, float dy) {
	i--;
//	cout << "(" << x << ", " << y << ", " << i << ")" << endl;
	setcolor(BLACK);
	if (ptr->left() != nullptr) {
		setcolor(RED);
		drawline(x, y, x - 20*(1 << i), y - dy);
		drawTree(ptr->left(), x - 20 * (1 << i), y - dy, i, dy);
	}
	if (ptr->right() != nullptr) {
		setcolor(RED);
		drawline(x, y, x + 20 * (1 << i), y - dy);
		drawTree(ptr->right(), x + 20 * (1 << i), y - dy, i, dy);
	}
	drawtext(x, y, std::to_string(ptr->getValue()), FLT_MAX, FLT_MAX);
}
