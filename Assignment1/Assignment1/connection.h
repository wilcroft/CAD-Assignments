#ifndef __ASSIGNMENT1_CONNECTION_H_
#define __ASSIGNMENT1_CONNECTION_H_

#include <iostream>
#include "segment.h"

class Connection {
private:
	int x1;
	int y1;
	int p1;
	int x2;
	int y2;
	int p2;

public:
	Connection();
	Connection(int p[6]);
	Connection(int, int, int, int, int, int);
	~Connection();
	bool operator==(const Connection&);
	bool isEOPL();
	void print();
	pin src();
	pin dest();
};

static const Connection EOPL = { -1, -1, -1, -1, -1, -1 };
#endif