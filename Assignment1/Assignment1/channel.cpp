#include "channel.h"

Channel::Channel(int n, int w){
	N = n;
	W = w;
//	mode = BIDIR;

	horiz = new Segment**[N];
	vert = new Segment  **[N + 1];

	for (int i = 0; i < N; i++){
		horiz[i] = new Segment*[N + 1];
		for (int j = 0; j < N + 1; j++){
			horiz[i][j] = new Segment[W];
			for (int k = 0; k < W; k++)
				horiz[i][j][k].setIndex(std::make_tuple('h', i, j, k));
		}
	}

	for (int i = 0; i < N+1; i++){
		vert[i] = new Segment*[N];
		for (int j = 0; j < N; j++){
			vert[i][j] = new Segment[W];
			for (int k = 0; k < W; k++)
				vert[i][j][k].setIndex(std::make_tuple('v', i, j, k));
		}
	}

}

Channel::~Channel(){
	for (int i = 0; i < N + 1; i++){
		for (int j = 0; j < N; j++){
			delete vert[i][j];
		}
		delete vert[i];
	}
	delete vert;
	for (int i = 0; i < N ; i++){
		for (int j = 0; j < N +1 ; j++){
			delete horiz[i][j];
		}
		delete horiz[i];
	}
	delete horiz;

}

void Channel::resetRouting(){
	for (int i = 0; i < N + 1; i++){
		for (int j = 0; j < N; j++){
			for (int k = 0; k < W; k++){
				vert[i][j][k].clear();
			}
		}
	}

	for (int i = 0; i < N; i++){
		for (int j = 0; j < N + 1; j++){
			for (int k = 0; k < W; k++){
				horiz[i][j][k].clear();
			}
		}
	}

}

void Channel::clearAttempt(){
	Segment * seg;
	for (int i = 0; i < N + 1; i++){
		for (int j = 0; j < N; j++){
			for (int k = 0; k < W; k++){
				seg = &vert[i][j][k];
				seg->setLength(MAXSEGLENGTH);
				if (seg->isRouting()||seg->isTarget()) seg->clear();
			}
		}
	}

	for (int i = 0; i < N; i++){
		for (int j = 0; j < N + 1; j++){
			for (int k = 0; k < W; k++){
				seg = &horiz[i][j][k];
				seg->setLength(MAXSEGLENGTH);
				if (seg->isRouting() || seg->isTarget()) seg->clear();
			}
		}
	}

}

bool Channel::findSetAvailableNeighbours(wire t, std::list<wire> * neigh){
	neigh->clear(); 
	int x = std::get<1>(t);
	int y = std::get<2>(t);
	int w = std::get<3>(t);
	pin p = horiz[x][y][w].getSource();
	Segment * seg;
	if (std::get<0>(t) = 'h'){
		seg = &horiz[x][y][w];
		if (x>0 && horiz[x - 1][y][w].isTarget()){
			horiz[x - 1][y][w].setLength(seg->getLength() + 1);
			return true;
		}
	//		|| horiz[x - 1][y][w].isUnused() || horiz[x - 1][y][w].usesSameSource(p))){
			//mark
		//}
	}
	else {
		seg = &vert[x][y][w];

	}
	return false;
}

/*  
 Marks neighbouring segments to t as length+1 if available or share the same source
 All marked segments are stored to the list neigh for further processing.
 Returns a pointer to a target segment, if found. Otherwise returns nullptr.
 t - pointer to segment to test
 neigh - list of available neighbours to t
 */
Segment * Channel::findSetAvailableNeighbours(Segment * t, std::list<Segment *> * neigh){
	neigh->clear();
	pin src = t->getSource();
	//int len = t->getLength();
	char hv = std::get<0>(t->getIndex());
	int x = std::get<1>(t->getIndex());
	int y = std::get<2>(t->getIndex());
	int w = std::get<3>(t->getIndex());
	enum checkReturn csStatus;
	if (MODE == BIDIR){
		if (hv == 'h'){
			if (x > 0){ // horiz left
				csStatus = segmentAt('h', x - 1, y, w)->checkAndSet(t); 
				if (csStatus == ISTARGET)
					return segmentAt('h', x - 1, y, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('h', x - 1, y, w)); //Segment marked, add to list
			}
			if (x <(N - 1)){ //horiz right
				csStatus = segmentAt('h', x + 1, y, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('h', x + 1, y, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('h', x + 1, y, w)); //Segment marked, add to list
			}
			if (y > 0){
				csStatus = segmentAt('v', x , y - 1, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('v', x , y - 1, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('v', x, y - 1, w)); //Segment marked, add to list

				csStatus = segmentAt('v', x + 1, y - 1, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('v', x + 1, y - 1, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('v', x + 1, y - 1, w)); //Segment marked, add to list
			}
			if (y < N){
				csStatus = segmentAt('v', x, y, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('v', x, y, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('v', x, y, w)); //Segment marked, add to list

				csStatus = segmentAt('v', x + 1, y, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('v', x + 1, y, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('v', x + 1, y, w)); //Segment marked, add to list

			}

		}
		else{
			if (y > 0){
				csStatus = segmentAt('v', x, y - 1, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('v', x, y - 1, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('v', x, y - 1, w)); //Segment marked, add to list
			}
			if (y < (N - 1)){
				csStatus = segmentAt('v', x , y + 1, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('v', x , y + 1, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('v', x , y + 1, w)); //Segment marked, add to list
			}
			if (x > 0){
				csStatus = segmentAt('h', x - 1, y, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('h', x - 1, y, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('h', x - 1, y, w)); //Segment marked, add to list

				csStatus = segmentAt('h', x - 1, y+1, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('h', x - 1, y+1, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('h', x - 1, y+1, w)); //Segment marked, add to list
			}
			if (x<N){
				csStatus = segmentAt('h', x, y, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('h', x, y, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('h', x, y, w)); //Segment marked, add to list

				csStatus = segmentAt('h', x, y + 1, w)->checkAndSet(t);
				if (csStatus == ISTARGET)
					return segmentAt('h', x, y + 1, w); //Target Found!
				if (csStatus == OK)
					neigh->push_back(segmentAt('h', x, y+1, w)); //Segment marked, add to list
			}
		}
	}
	else{

	}
	return nullptr;
}

bool Channel::route(pin src, pin dest){
	//Mark available destination segments as targets
	bool isOK = false;
	for (int i = 0; i < W; i++){
		Segment* seg = segmentAt(dest, i);
		if (seg->isUnused()){
			seg->setState(TARGET);
			isOK = true;
		}
	}

	//No destination segments available - return a failure
	if (!isOK)
		return false;
	else isOK = false;

	//event_loop(NULL, NULL, NULL, drawscreen);
	std::list<Segment*> expansion;
	std::list<Segment*> temp;

	//Test source segments
	for (int i = 0; i < W; i++){
		Segment* seg = segmentAt(src, i);
		//Segment is also a target segment - done!
		if (seg->isTarget()){
			seg->setSource(src);
			seg->setState(USED);
			seg->setDest(dest);
			return true;
		}
		//Set unused or common-source segments to l=0 and add to list
		if (seg->isUnused() || (seg->isUsed() && seg->getSource() == src)){
			isOK = true;
			seg->setLength(0);
			seg->setSource(src);
			expansion.push_back(seg);
			if (seg->isUnused()) seg->setState(ROUTING);
		}
	}
	//No available segments next to source pin
	if (!isOK)
		return false;

	Segment* toProcess, * trace=nullptr;
	while (!expansion.empty() && trace==nullptr){
		toProcess = expansion.front();
		expansion.pop_front();
		trace=findSetAvailableNeighbours(toProcess, &temp);
		expansion.splice(expansion.end(), temp);
	}
	if (trace == nullptr) return false; // No path from source to dest found

	//traceback
	trace->setSource(src);
	trace->setDest(dest);
	traceback(trace);


	//event_loop(NULL, NULL, NULL, drawscreen);
	return true;

}
Segment* Channel::segmentAt(pin p, int w){
	switch (std::get<2>(p)){
	case 1:
		return &(horiz[std::get<0>(p)][std::get<1>(p)][w]);
	case 2:
		return &(vert[std::get<0>(p)+1][std::get<1>(p)][w]);
	case 3:
		return &(horiz[std::get<0>(p)][std::get<1>(p)+1][w]);
	case 4:
		return &(vert[std::get<0>(p)][std::get<1>(p)][w]);
	default:
		return nullptr;
	}

}
Segment* Channel::segmentAt(bool horiz, int x, int y, int w){
	if (horiz)
		return &(this->horiz[x][y][w]);
	else
		return &(vert[x][y][w]);
}
Segment* Channel::segmentAt(char hv, int x, int y, int w){
	if (hv=='h')
		return &(this->horiz[x][y][w]);
	else
		return &(vert[x][y][w]);
}
Segment* Channel::segmentAt(wire w){
	if (std::get<0>(w) == 'h')
		return &(horiz[std::get<1>(w)][std::get<2>(w)][std::get<3>(w)]);
	else
		return &(vert[std::get<1>(w)][std::get<2>(w)][std::get<3>(w)]);
}

/*
Marks back from a destination pin to source pin
dest - destination
*/
void Channel::traceback(Segment * dest){
	pin src = dest->getSource();
	//int len = t->getLength();
	char hv = std::get<0>(dest->getIndex());
	int x = std::get<1>(dest->getIndex());
	int y = std::get<2>(dest->getIndex());
	int w = std::get<3>(dest->getIndex());
	int l = dest->getLength();
	if (l == 0) return;

	if (MODE == BIDIR){
		if (hv == 'h'){
		//First pass - look for already used segments
			if (x > 0 && segmentAt('h', x - 1, y, w)->getLength() == l - 1 
					&& segmentAt('h', x - 1, y, w)->isUsed()){
				traceback(segmentAt('h', x - 1, y, w));
			}
			else if (x < (N - 1) && segmentAt('h', x + 1, y, w)->getLength() == l - 1
					&& segmentAt('h', x + 1, y, w)->isUsed()){
				traceback(segmentAt('h', x + 1, y, w));
			}
			else if (y > 0 && segmentAt('v', x, y - 1, w)->getLength() == l - 1
					&& segmentAt('v', x, y - 1, w)->isUsed()){
				traceback(segmentAt('v', x, y - 1, w));
			}
			else if (y > 0 && segmentAt('v', x + 1, y - 1, w)->getLength() == l - 1
					&& segmentAt('v', x + 1, y - 1, w)->isUsed()){
				traceback(segmentAt('v', x + 1, y - 1, w));
			}
			else if (y < N && segmentAt('v', x, y, w)->getLength() == l - 1
					&& segmentAt('v', x, y, w)->isUsed()){
				traceback(segmentAt('v', x, y, w));
			}
			else if (y < N && segmentAt('v', x + 1, y, w)->getLength() == l - 1
					&& segmentAt('v', x + 1, y, w)->isUsed()){
				traceback(segmentAt('v', x + 1, y, w));
			}
		//Second Pass - take any route
			else if (x > 0 && segmentAt('h', x - 1, y, w)->getLength() == l - 1){
				segmentAt('h', x - 1, y, w)->setState(USED);
				segmentAt('h', x - 1, y, w)->setSource(src);
				traceback(segmentAt('h', x - 1, y, w));
			}
			else if (x < (N - 1) && segmentAt('h', x + 1, y, w)->getLength() == l - 1){
				segmentAt('h', x + 1, y, w)->setState(USED);
				segmentAt('h', x + 1, y, w)->setSource(src);
				traceback(segmentAt('h', x + 1, y, w));
			}
			else if (y > 0 && segmentAt('v', x, y - 1, w)->getLength() == l - 1){
				segmentAt('v', x, y - 1, w)->setState(USED);
				segmentAt('v', x, y - 1, w)->setSource(src);
				traceback(segmentAt('v', x, y - 1, w));
			}
			else if (y > 0 && segmentAt('v', x + 1, y - 1, w)->getLength() == l - 1){
				segmentAt('v', x + 1, y - 1, w)->setState(USED);
				segmentAt('v', x + 1, y - 1, w)->setSource(src);
				traceback(segmentAt('v', x + 1, y - 1, w));
			}
			else if (y < N && segmentAt('v', x, y, w)->getLength() == l - 1){
				segmentAt('v', x, y, w)->setState(USED);
				segmentAt('v', x, y, w)->setSource(src);
				traceback(segmentAt('v', x, y, w));
			}
			else if (y < N && segmentAt('v', x + 1, y, w)->getLength() == l - 1){
				segmentAt('v', x + 1, y, w)->setState(USED);
				segmentAt('v', x + 1, y, w)->setSource(src);
				traceback(segmentAt('v', x + 1, y, w));
			}
		}
		else{
			//First Pass - try to reuse segements
			if (y > 0 && segmentAt('v', x, y - 1, w)->getLength() == l - 1
					&& segmentAt('v', x, y - 1, w)->isUsed()){
				traceback(segmentAt('v', x, y - 1, w));
			}
			else if (y < (N - 1) && segmentAt('v', x, y + 1, w)->getLength() == l - 1
					&& segmentAt('v', x, y + 1, w)->isUsed()){
				traceback(segmentAt('v', x, y + 1, w)); 
			}
			else if (x > 0 && segmentAt('h', x - 1, y, w)->getLength() == l - 1
					&& segmentAt('h', x - 1, y, w)->isUsed()){
				traceback(segmentAt('h', x - 1, y, w));
			}
			else if (x>0 && segmentAt('h', x - 1, y + 1, w)->getLength() == l - 1
					&& segmentAt('h', x - 1, y + 1, w)->isUsed()){
				traceback(segmentAt('h', x - 1, y + 1, w));
			}
			else if (x < N && segmentAt('h', x, y, w)->getLength() == l - 1
					&& segmentAt('h', x, y, w)->isUsed()){
				traceback(segmentAt('h', x, y, w));
			}
			else if (x<N && segmentAt('h', x, y + 1, w)->getLength() == l - 1
					&& segmentAt('h', x, y + 1, w)->isUsed()){
				traceback(segmentAt('h', x, y + 1, w));
			}
			//Second Pass - no segments to reuse
			if (y > 0 && segmentAt('v', x, y - 1, w)->getLength() == l - 1){
				segmentAt('v', x, y - 1, w)->setState(USED);
				segmentAt('v', x, y - 1, w)->setSource(src);
				traceback(segmentAt('v', x, y - 1, w));
			}
			else if (y < (N - 1) && segmentAt('v', x, y + 1, w)->getLength() == l - 1){
				segmentAt('v', x, y + 1, w)->setState(USED);
				segmentAt('v', x, y + 1, w)->setSource(src);
				traceback(segmentAt('v', x, y + 1, w));
			}
			else if (x > 0 && segmentAt('h', x - 1, y, w)->getLength() == l - 1){
				segmentAt('h', x - 1, y, w)->setState(USED);
				segmentAt('h', x - 1, y, w)->setSource(src);
				traceback(segmentAt('h', x - 1, y, w));
			}
			else if (x>0 && segmentAt('h', x - 1, y + 1, w)->getLength() == l - 1){
				segmentAt('h', x - 1, y + 1, w)->setState(USED);
				segmentAt('h', x - 1, y + 1, w)->setSource(src);
				traceback(segmentAt('h', x - 1, y + 1, w));
			}
			else if (x < N && segmentAt('h', x, y, w)->getLength() == l - 1){
				segmentAt('h', x, y, w)->setState(USED);
				segmentAt('h', x, y, w)->setSource(src);
				traceback(segmentAt('h', x, y, w));
			}
			else if (x<N && segmentAt('h', x, y + 1, w)->getLength() == l - 1){
				segmentAt('h', x, y + 1, w)->setState(USED);
				segmentAt('h', x, y + 1, w)->setSource(src);
				traceback(segmentAt('h', x, y + 1, w));
			}
		}
	}
	else{
		//The unidirectional case!
	}
	return;
}