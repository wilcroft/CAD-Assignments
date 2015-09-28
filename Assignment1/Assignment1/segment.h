#ifndef __ASSIGNMENT1_SEGMENT_H_
#define __ASSIGNMENT1_SEGMENT_H_
#include <tuple>

#define MAXSEGLENGTH 8191

typedef std::tuple<int, int, int> pin;
typedef std::tuple<char, int, int, int> wire;

enum segment_state{
	UNUSED,
	USED,
	TARGET,
	ROUTING
};

enum checkReturn{
	OK,
	ISTARGET,
	ISUSED
};

class Segment{
private:
	enum segment_state state;
	int length;
	pin sourcepin;
	pin destpin;
	wire index;
public:
	Segment();
	Segment(wire w);
	Segment(char h, int x, int y, int w);
	void clear();
	bool isTarget();
	bool isUnused();
	bool isUsed();
	bool isRouting();
	bool usesSameSource(pin p);
	bool usesSameSource(Segment* s);
	enum checkReturn checkAndSet(Segment * src);

	void setState(enum segment_state s);
	void setLength(int l);
	void setSource(pin s);
	void setDest(pin d);
	void setIndex(wire w);

	pin getSource();
	pin getDest();
	int getLength();
	enum segment_state getState();
	wire getIndex();

};

#endif