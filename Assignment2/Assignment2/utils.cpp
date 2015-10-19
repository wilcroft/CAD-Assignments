#include "utils.h"

namespace commonvars{
	int graphn, graphw;
	t_bound_box initial_coords;
	int maxNetNum = 0;
	int numFreeBlocks = 0;
	std::list<Block> allBlocks;
	std::list<Net> allNets;
}

int parseInputFile(char * fname){
	std::ifstream fs(fname);
	string temp;
	int blocknum;
	int tempint;
	std::list<int> templist;
	//Block * blk;
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
			templist.clear();
			do {
				//Get nets for the block number
				try { tempint = std::stoi(temp, &idx); } //ensure conversion from string to int
				catch (const std::invalid_argument& ia) {
					cerr << "Error: '" << temp << "' is not a valid decimal integer" << endl;
					return -1;
				}
				temp.erase(0, idx);
				if (tempint != -1) templist.push_back(tempint);
				if (tempint > commonvars::maxNetNum) commonvars::maxNetNum = tempint;
			} while (tempint != -1);
			commonvars::allBlocks.emplace_back(blocknum, templist);
		}
	} while (blocknum != -1);
	
	commonvars::numFreeBlocks = commonvars::allBlocks.size();

	//Read the Blocks + Nets
	do {
		std::getline(fs, temp);
		if (fs.eof()) {
			cerr << "Error: block location list in file \"" << fname << "\" should be terminated with -1" << endl;
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
			std::list<Block>::iterator it = commonvars::allBlocks.begin();
			for (; it != commonvars::allBlocks.end() && it->getBlockNum() != blocknum; it++);
			//x position
			try { tempint = std::stoi(temp, &idx); } //ensure conversion from string to int
			catch (const std::invalid_argument& ia) {
				cerr << "Error: '" << temp << "' is not a valid decimal integer" << endl;
				return -1;
			}
			it->setX(tempint);

			temp.erase(0, idx);
			//y position
			try { tempint = std::stoi(temp, &idx); } //ensure conversion from string to int
			catch (const std::invalid_argument& ia) {
				cerr << "Error: '" << temp << "' is not a valid decimal integer" << endl;
				return -1;
			}
			temp.erase(0, idx);
			it->setY(tempint);
			it->setFixed();
			commonvars::numFreeBlocks--;
		}
	} while (blocknum != -1);

	fs.close();
	//connlist.unique(&Connection::operator==);
	
	return 0;
}

void initialPlace(std::list<Block> * blocks) {
	std::vector<double> bx;
	std::vector<double> by;
	std::vector<int> Ap;
	std::vector<int> Ai;
	std::vector<double> A;
	std::vector<double> x;
	std::vector<double> y;

	bx.resize(commonvars::numFreeBlocks);
	by.resize(commonvars::numFreeBlocks);
	x.resize(commonvars::numFreeBlocks);
	y.resize(commonvars::numFreeBlocks);
	Ap.resize(commonvars::numFreeBlocks+1);
	Ap[0] = 0;

	unsigned int i = 0;
	//for (std::list<Block>::iterator ita = blocks->begin(); ita != blocks->end(); ita++) {
	for (auto& blki:*blocks){
		int j = 0;
		if (!blki.isFixed()) {
			Ap[i + 1] = Ap[i];
			for (auto& blkj : *blocks) {
				if (&blki == &blkj) {
					Ap[i + 1]++;
					Ai.push_back(j);
					A.push_back(blki.getSumWeights());
					j++;
				}
				else if (blkj.isFixed()) {
					bx[i] += blki.getWeight(&blkj)*blkj.getX();
					by[i] += blki.getWeight(&blkj)*blkj.getY();
				}
				else {
					double w = blki.getWeight(&blkj);
					if (w != 0) {
						Ap[i + 1]++;
						Ai.push_back(j);
						A.push_back(-w);
					}
					j++;
				}
			}

			i++;
		}
	}

	cout << "Ap = [";
	for (i = 0; i < Ap.size(); i++) {
		cout << Ap[i] << " ";
	}
	cout << "]" << endl;
	cout << "Ai = [";
	for (i = 0; i < Ai.size(); i++) {
		cout << Ai[i] << " ";
	}
	cout << "]" << endl;
	cout << "A  = [";
	for (i = 0; i < A.size(); i++) {
		cout << A[i] << " ";
	}
	cout << "]" << endl;
	cout << "bx = [";
	for (i = 0; i < bx.size(); i++) {
		cout << bx[i] << " ";
	}
	cout << "]" << endl;
	cout << "by = [";
	for (i = 0; i < by.size(); i++) {
		cout << by[i] << " ";
	}
	cout << "]" << endl;


	void * Symbolic, * Numeric;

	(void)umfpack_di_symbolic(commonvars::numFreeBlocks, commonvars::numFreeBlocks, Ap.data(), Ai.data(), A.data(), &Symbolic, nullptr, nullptr);
	(void)umfpack_di_numeric(Ap.data(), Ai.data(), A.data(), Symbolic, &Numeric, nullptr, nullptr);
	umfpack_di_free_symbolic(&Symbolic);
	(void)umfpack_di_solve(UMFPACK_A, Ap.data(), Ai.data(), A.data(), x.data(), bx.data(), Numeric, nullptr, nullptr);
	(void)umfpack_di_solve(UMFPACK_A, Ap.data(), Ai.data(), A.data(), y.data(), by.data(), Numeric, nullptr, nullptr);
	umfpack_di_free_numeric(&Numeric);
	cout << "y = [";
	for (i = 0; i < y.size(); i++) {
		cout << y[i] << " ";
	}
	cout << "]" << endl;
	cout << "x = [";
	for (i = 0; i < x.size(); i++) {
		cout << x[i] << " ";
	}
	cout << "]" << endl;

    i=0;
    for (auto& b:*blocks){
        if (!b.isFixed()){
            b.setX(bx[i]);
            b.setY(by[i]);
            i++;
        }
    }
	
}

int wireusage (std::list<Net> * nets){
    int sum = 0;
    for(auto& n:*nets){
        sum += n.HPWL();
    }
    return sum;
}

void drawscreen(){
	//extern int chipn, chipw;
	set_draw_mode(DRAW_NORMAL);
	clearscreen();

	setlinestyle(SOLID);
	setlinewidth(1);

	setcolor(LIGHTGREY);
	
	//int subsq = 2 * commonvars::graphw + 1;

	//for (int i = 0; i < commonvars::graphn; i++){
	//	for (int j = 0; j < commonvars::graphn + 1; j++){
	//		for (int k = 0; k < commonvars::graphw; k++){
	//			//Draw the wires
	//			setcolor(LIGHTGREY);
	//			//drawline(subsq*j * 2 + 2 * k + 1, (i + 1) * 2 * subsq - 1, subsq*j * 2 + 2 * k + 1, (2 * i + 1)*subsq);
	//			//drawline( (i + 1) * 2 * subsq - 1,subsq*j * 2 + 2 * k + 1, (2 * i + 1)*subsq, subsq*j * 2 + 2 * k + 1);
	//			drawWireSegment(true, i, j, k, LIGHTGREY);
	//			drawWireSegment(false, j, i, k, LIGHTGREY);
	//		}
	//	}
	//	for (int j = 0; j < commonvars::graphn; j++){
	//		//Draw the logic blocks
	//		setcolor(DARKGREY);
	//		fillrect((2 * i + 1)*subsq, (2 * j + 1)*subsq, 2 *(i+1)*subsq - 1, 2 *(j+1)*subsq - 1);
	//		for (int k = 1; k < 5; k++){
	//			pin p = std::make_tuple(i, j, k);
	//			bool ok = p == std::make_tuple(0, 2, 4);
	//			for (int w = 0; w < commonvars::graphw; w++){
	//				if (commonvars::routing->segmentAt(p, w)->getSource() == p)
	//					drawPinToWire(p, w);
	//				if (commonvars::routing->segmentAt(p, w)->getDest() == p)
	//					drawPinToWire(p, w, commonvars::colormap[commonvars::routing->segmentAt(p, w)->getSource()]);
	//					
	//			}
	//		}
	//	}
	//}
}

/*
std::list<Segment *> randomizeList(std::list<Segment *> l){
	std::list<Segment *> newl;
	std::vector<Segment *> v;
	for(std::list<Segment *>::iterator iter = l.begin(); iter!= l.end(); iter++){
		v.push_back(*iter);
	}
	std::random_shuffle(v.begin(), v.end());
	for (int i=0; i<v.size(); i++)
		newl.push_back(v[i]);

	return newl;

}


void popToFront(std::list<struct connections_t>* lst, std::list<struct connections_t>::iterator it) {
	struct connections_t temp;

	temp = *(it);
	lst->erase(it);
	lst->push_front(temp);

}

void popToFront(std::list<Connection>* lst, std::list<Connection>::iterator it) {
	Connection temp;

	temp = *(it);
	lst->erase(it);
	lst->push_front(temp);

}
*/
