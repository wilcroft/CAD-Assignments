#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <list>
#include <cfloat>

#include "block.h"
#include "tree.h"
#include "graphics.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;


int parseInputFile(char * fname);
std::list <Block *> getOrderedList(std::vector<Block> &b);
void doBandB(std::vector<Block> &blocks);
void exploreTree(std::list<Block*> &blocks, std::list<Block*>::iterator it, int currCost, int& bestCost, int lcount, int rcount, const int maxcount, Tree * treenode);
int initialCost(Block * b, int maxcount);
int initialCostNet(int maxcount);

void drawscreen();
void drawTree(Tree * ptr, float x, float y, int i, float dy);

