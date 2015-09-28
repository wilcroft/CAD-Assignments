#ifndef __ASSIGNMENT1__UTILS_H_
#define __ASSIGNMENT1__UTILS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <list>

#define WIN32 1

#include <map>

#include "connection.h"
#include "graphics.h"
#include "channel.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace utilvars{
	extern int graphn, graphw;
	extern t_bound_box initial_coords;
	extern Channel * routing;
	extern std::map<pin, enum color_types> colormap;
}

struct connections_t {
	int p[6]; // {x1, y1, p1, x2, y2, p2}
};

//const struct connections_t EOPL = { -1, -1, -1, -1, -1, -1 };

void popToFront(std::list<struct connections_t>* lst, std::list<struct connections_t>::iterator it);
void popToFront(std::list<Connection>* lst, std::list<Connection>::iterator it);
bool isEOPL(struct connections_t conn);
int parseInputFile(char * fname, int * n, int * w, std::list<Connection> * connlist);
void printConnList(std::list<Connection> connlist);
void printPin(pin p);

void drawscreen();
void drawWireSegment(bool isHoriz, int x, int y, int w, enum color_types c);
void drawSwitchConnections(bool isHoriz, int x, int y, int w);
void drawPinToWire(pin p, int w, enum color_types c = NUM_COLOR);
#endif