#include "utils.h"

namespace utilvars{
	int graphn, graphw;
	t_bound_box initial_coords;
	Channel * routing;
	std::map<pin, enum color_types> colormap;
}

void popToFront(std::list<struct connections_t>* lst, std::list<struct connections_t>::iterator it){
	struct connections_t temp;

	temp = *(it);
	lst->erase(it);
	lst->push_front(temp);

}

void popToFront(std::list<Connection>* lst, std::list<Connection>::iterator it){
	Connection temp;

	temp = *(it);
	lst->erase(it);
	lst->push_front(temp);

}

void printPin(pin p){
	cout << std::get<0>(p) << ", " << std::get<1>(p) << ", " << std::get<2>(p) << endl;
}

int parseInputFile(char * fname, int * n, int * w, std::list<Connection> * connlist){
	std::ifstream fs(fname);
	string temp;
	int item [6];
	size_t idx = 0;

	if (fs.fail()) {
		cerr << "Error: Couldn't open file \"" << fname << "\"" << endl;
		return -1;
	}

	//get the value for n
	std::getline(fs, temp);
	if (fs.eof()) {
		cerr << "Error: missing parameter 'n' in file \"" << fname << "\"" << endl;
		return -1;
	}
	try{ *n = std::stoi(temp); } //ensure conversion from string to int
	catch (const std::invalid_argument& ia) {
		cerr << "Error: '" << temp << "' is not a valid decimal integer" << endl;
		return -1;
	}
	cout << "n=" << temp << endl;
	utilvars::graphn = *n;

	//get the value for w
	std::getline(fs, temp);
	if (fs.eof()) {
		cerr << "Error: missing parameter 'w' in file \"" << fname << "\"" << endl;
		return -1;
	}
	try{ *w = std::stoi(temp); }
	catch (const std::invalid_argument& ia) {
		cerr << "Error: '" << temp << "' is not a valid decimal integer" << endl;
		return -1;
	}
	cout << "w=" << temp << endl;
	utilvars::graphw = *w;

	std::getline(fs, temp);
	while (!fs.eof()){
		cout << "'" << temp << "'" << endl;
		for (int i = 0; i < 6; i++){
			try{ item[i] = std::stoi(temp, &idx); }
			catch (const std::invalid_argument& ia) {
				cerr << "Error: not a valid decimal integer" << endl;
				return -1;
			}
			temp.erase(0, idx);
		}
		Connection newconn(item);
		if (newconn.isEOPL()) break;
		connlist->push_back(newconn);
		std::getline(fs, temp);
	}

	fs.close();
	//connlist.unique(&Connection::operator==);
	
	return 0;
}

void printConnList(std::list<Connection> connlist){
	std::list<Connection>::iterator it = connlist.begin();

	for (it; it != connlist.end(); it++){
		it->print();
	}

}

void drawscreen(){
	//extern int chipn, chipw;
	set_draw_mode(DRAW_NORMAL);
	clearscreen();

	setlinestyle(SOLID);
	setlinewidth(1);

	setcolor(LIGHTGREY);
	
	int subsq = 2 * utilvars::graphw + 1;

	for (int i = 0; i < utilvars::graphn; i++){
		for (int j = 0; j < utilvars::graphn + 1; j++){
			for (int k = 0; k < utilvars::graphw; k++){
				//Draw the wires
				setcolor(LIGHTGREY);
				//drawline(subsq*j * 2 + 2 * k + 1, (i + 1) * 2 * subsq - 1, subsq*j * 2 + 2 * k + 1, (2 * i + 1)*subsq);
				//drawline( (i + 1) * 2 * subsq - 1,subsq*j * 2 + 2 * k + 1, (2 * i + 1)*subsq, subsq*j * 2 + 2 * k + 1);
				drawWireSegment(true, i, j, k, LIGHTGREY);
				drawWireSegment(false, j, i, k, LIGHTGREY);
			}
		}
		for (int j = 0; j < utilvars::graphn; j++){
			//Draw the logic blocks
			setcolor(DARKGREY);
			fillrect((2 * i + 1)*subsq, (2 * j + 1)*subsq, 2 *(i+1)*subsq - 1, 2 *(j+1)*subsq - 1);
			for (int k = 1; k < 5; k++){
				pin p = std::make_tuple(i, j, k);
				for (int w = 0; w < utilvars::graphw; w++){
					if (utilvars::routing->segmentAt(p, w)->getSource() == p)
						drawPinToWire(p, w);
					if (utilvars::routing->segmentAt(p, w)->getDest() == p)
						drawPinToWire(p, w, utilvars::colormap[utilvars::routing->segmentAt(p, w)->getSource()]);
						
				}
			}
		}
	}
}


void drawWireSegment(bool isHoriz, int x, int y, int w, enum color_types c){
	int subsq = 2 * utilvars::graphw + 1;

	setcolor(c);
	if (utilvars::routing->segmentAt(isHoriz, x, y, w)->getState() == ROUTING) setcolor(YELLOW);
	if (utilvars::routing->segmentAt(isHoriz, x, y, w)->getState() == TARGET) setcolor(ORANGE);
	if (utilvars::routing->segmentAt(isHoriz, x, y, w)->getState() == USED) {
		setcolor(utilvars::colormap[utilvars::routing->segmentAt(isHoriz, x, y, w)->getSource()]);
		drawSwitchConnections(isHoriz, x, y, w);
	}
	if (isHoriz){
		drawline((x + 1) * 2 * subsq - 1, subsq*y * 2 + 2 * w + 1, (2 * x + 1)*subsq, subsq*y * 2 + 2 * w + 1);
	}
	else{
		drawline(subsq*x * 2 + 2 * w + 1, (y + 1) * 2 * subsq - 1, subsq*x * 2 + 2 * w + 1, (2 * y + 1)*subsq);
	}

}

void drawSwitchConnections(bool isHoriz, int x, int y, int w){

	Segment * seg = utilvars::routing->segmentAt(isHoriz, x, y, w);
	if (!seg->isUsed()) return;
	
	pin src = seg->getSource();
	bool bp = (src == std::make_tuple(0, 2, 4));
	int subsq = 2 * utilvars::graphw + 1;
	setcolor(utilvars::colormap[src]);

	if (MODE == BIDIR){
		if (isHoriz){
			//left side
			if (x > 0 && utilvars::routing->segmentAt('h', x - 1, y, w)->getSource() == src && utilvars::routing->segmentAt('h', x - 1, y, w)->isUsed()){
				drawline((2 * x + 1)*subsq, subsq*y * 2 + 2 * w + 1, (x) * 2 * subsq - 1, subsq*y * 2 + 2 * w + 1);
			}
			if (y > 0 && utilvars::routing->segmentAt('v', x, y - 1, w)->getSource() == src && utilvars::routing->segmentAt('v', x, y - 1, w)->isUsed()){
				drawline((2 * x + 1)*subsq, subsq*y * 2 + 2 * w + 1, subsq*x * 2 + 2 * w + 1, (y) * 2 * subsq - 1);
			}
			if (y < utilvars::graphn && utilvars::routing->segmentAt('v', x, y, w)->getSource() == src && utilvars::routing->segmentAt('v', x, y, w)->isUsed()){
				drawline((2 * x + 1)*subsq, subsq*y * 2 + 2 * w + 1, subsq*x * 2 + 2 * w + 1, (2 * y + 1)*subsq);
			}
			//right side
			if (x < (utilvars::graphn - 1) && utilvars::routing->segmentAt('h', x + 1, y, w)->getSource() == src && utilvars::routing->segmentAt('h', x + 1, y, w)->isUsed()){
			//	drawline((x + 1) * 2 * subsq - 1, subsq*y * 2 + 2 * w + 1, (x - 1) * 2 * subsq - 1, subsq*y * 2 + 2 * w + 1);
			}
			if (y > 0 && utilvars::routing->segmentAt('v', x + 1, y - 1, w)->getSource() == src && utilvars::routing->segmentAt('v', x + 1, y - 1, w)->isUsed()){
				drawline((x + 1) * 2 * subsq - 1, subsq*y * 2 + 2 * w + 1, subsq*(x+1) * 2 + 2 * w + 1, (y)* 2 * subsq - 1);
			}
			if (y < utilvars::graphn && utilvars::routing->segmentAt('v', x + 1, y, w)->getSource() == src && utilvars::routing->segmentAt('v', x + 1, y, w)->isUsed()){
				drawline((x + 1) * 2 * subsq - 1, subsq*y * 2 + 2 * w + 1, subsq*(x+1) * 2 + 2 * w + 1, (2 * y + 1)*subsq);
			}

		}
		else{
			if (y > 0 && utilvars::routing->segmentAt('v', x, y - 1, w)->getSource() == src && utilvars::routing->segmentAt('v', x, y - 1, w)->isUsed())
				drawline(subsq*x * 2 + 2 * w + 1, (2 * y + 1)*subsq, subsq*x * 2 + 2 * w + 1, (y)* 2 * subsq - 1);
		}

	}
	else{

	}
}

void drawPinToWire(pin p, int w, enum color_types c){
	if (c==NUM_COLOR)	setcolor(utilvars::colormap[p]);
	else				setcolor(c);
	int x = std::get<0>(p);
	int y = std::get<1>(p);
	int o = std::get<2>(p);
	int subsq = 2 * utilvars::graphw + 1;

	//fillrect((2 * i + 1)*subsq, (2 * j + 1)*subsq, 2 * (i + 1)*subsq - 1, 2 * (j + 1)*subsq - 1);
	
	switch (o){
	case 1:
		drawline((2 * x + 1)*subsq + 1, (2 * y + 1)*subsq, (2 * x + 1)*subsq + 1, subsq*y * 2 + 2 * w + 1);
		break; 
	case 2:
		drawline(2 * (x + 1)*subsq - 1, 2 * (y + 1)*subsq - 2, subsq*(x+1) * 2 + 2 * w + 1, 2 * (y + 1)*subsq - 2);
		break;
	case 3:
		drawline(2 * (x + 1)*subsq - 2, 2 * (y + 1)*subsq - 1, 2 * (x + 1)*subsq - 2, subsq*(y+1)* 2 + 2 * w + 1 );
		break;
	case 4:
		drawline((2 * x + 1)*subsq, (2 * y + 1)*subsq + 1, subsq*x * 2 + 2 * w + 1, (2 * y + 1)*subsq + 1);
	default:;
	}
}