#include "connection.h"

Connection::Connection(){
	x1 = 0;
	y1 = 0;
	p1 = 0;
	x2 = 0;
	y2 = 0;
	p2 = 0;
}

Connection::Connection(int p[6]){
	x1 = p[0];
	y1 = p[1];
	p1 = p[2];
	x2 = p[3];
	y2 = p[4];
	p2 = p[5];

}


Connection::Connection(int a, int b, int c, int d, int e, int f){
	x1 = a;
	y1 = b;
	p1 = c;
	x2 = d;
	y2 = e;
	p2 = f;
}
Connection::~Connection(){

}
bool Connection::operator==(const Connection& c){
	return (x1 == c.x1 && y1 == c.y1 && p1 == c.p1 && x2 == c.x2 && y2 == c.y2&& p2 == c.p2);

}
bool Connection::isEOPL(){
	//return (this == &EOPL);
	return (x1 == EOPL.x1 && y1 == EOPL.y1 && p1 == EOPL.p1 && x2 == EOPL.x2 && y2 == EOPL.y2&& p2 == EOPL.p2);
}

void Connection::print(){
	std::cout << "Source: (" << x1 << ", " << y1 << ", " << p1 << ") -> Dest: (" << x2 << ", " << y2 << ", " << p2 << ")" << std::endl;
}

pin Connection::src(){ return std::make_tuple(x1, y1, p1); }
pin Connection::dest(){ return std::make_tuple(x2, y2, p2); }