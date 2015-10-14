#include "utils.h"

namespace commonvars{
	int graphn, graphw;
	t_bound_box initial_coords;
	int maxNetNum = 0;
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
		}
	} while (blocknum != -1);

	fs.close();
	//connlist.unique(&Connection::operator==);
	
	return 0;
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