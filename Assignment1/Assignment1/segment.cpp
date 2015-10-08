#include "segment.h"

Segment::Segment(){
	state = UNUSED;
	sourcepin = std::make_tuple(-1, -1, -1);
	destpin = std::make_tuple(-1, -1, -1);
	length = MAXSEGLENGTH;
}
void Segment::clear(){
	state = UNUSED;
	sourcepin = std::make_tuple(-1, -1, -1);
	destpin = std::make_tuple(-1, -1, -1);
	length = MAXSEGLENGTH;

}
bool Segment::isTarget(){ return (state == TARGET); }
bool Segment::isUnused(){ return (state == UNUSED); }
bool Segment::isUsed(){ return (state == USED); }
bool Segment::isRouting(){ return (state == ROUTING); }
bool Segment::usesSameSource(pin p){ return (sourcepin == p); }
bool Segment::usesSameSource(Segment * s){ return (sourcepin == s->getSource()); }
enum checkReturn Segment::checkAndSet(Segment * src){
	if (state == TARGET){
		length = src->getLength() + 1;
		sourcepin = src->getSource();
		state = USED;	//????
		return ISTARGET;
	}
	if (state == UNUSED){
		length = src->getLength() + 1;
		sourcepin = src->getSource();
		state = ROUTING;
		return OK;
	}
	if (state == USED && usesSameSource(src)){
		length = src->getLength() + 1;
		return OK;
	}

	return ISUSED;

}

void Segment::setState(enum segment_state s){ state = s; }
void Segment::setLength(int l){ length = l; }
void Segment::setSource(pin s){ sourcepin = s; }
void Segment::setDest(pin d) { destpin = d; }
void Segment::setIndex(wire w){ index = w; }

pin Segment::getSource(){ return sourcepin; }
pin Segment::getDest(){ return destpin; }
int Segment::getLength(){ return length; }
enum segment_state Segment::getState(){ return state; }
wire Segment::getIndex(){ return index; }
