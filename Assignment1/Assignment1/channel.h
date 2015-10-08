#ifndef __ASSIGNMENT1_CHANNEL_H_
#define __ASSIGNMENT1_CHANNEL_H_

#include <list>
#include <tuple>
#include <queue>
#include <algorithm>

#include "segment.h"
#include "graphics.h"

enum CHANNELMODE{
	BIDIR,
	UNIDIR
};

#ifndef MODE
#define MODE BIDIR
#endif

class Channel{
private:
	Segment *** horiz;
	Segment *** vert;
	int N;
	int W;
	bool tryHard;
//	enum CHANNELMODE mode;
public:
	Channel(int n, int w);
	~Channel();

	void resetRouting();
	void clearAttempt();
	int routingSegmentsUsed();
	int maxW();
	bool findSetAvailableNeighbours(wire t, std::list<wire>* neigh);
	Segment * findSetAvailableNeighbours(Segment * t, std::list<Segment *> * neigh);
	bool route(pin src, pin dest);
	Segment* segmentAt(pin p, int w);
	Segment* segmentAt(bool horiz, int x, int y, int w);
	Segment* segmentAt(wire w);
	Segment* segmentAt(char hv, int x, int y, int w);
	void traceback(Segment * dest); 
	void tryHarder(bool b=true);

};

extern void drawscreen();
extern std::list<Segment *> randomizeList(std::list<Segment *> l);

#endif
