#ifndef __ASSIGNMENT1_CHANNEL_H_
#define __ASSIGNMENT1_CHANNEL_H_

#include <list>
#include <tuple>
#include <queue>

#include "segment.h"
#include "graphics.h"

enum CHANNELMODE{
	BIDIR,
	UNIDIR
};

#define MODE UNIDIR

class Channel{
private:
	Segment *** horiz;
	Segment *** vert;
	int N;
	int W;
//	enum CHANNELMODE mode;
public:
	Channel(int n, int w);
	~Channel();

	void resetRouting();
	void clearAttempt();
	bool findSetAvailableNeighbours(wire t, std::list<wire>* neigh);
	Segment * findSetAvailableNeighbours(Segment * t, std::list<Segment *> * neigh);
	bool route(pin src, pin dest);
	Segment* segmentAt(pin p, int w);
	Segment* segmentAt(bool horiz, int x, int y, int w);
	Segment* segmentAt(wire w);
	Segment* segmentAt(char hv, int x, int y, int w);

	void traceback(Segment * dest); //, Segment * src);

};

extern void drawscreen();

#endif