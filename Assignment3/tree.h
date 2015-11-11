#pragma once

// A basic binary tree class

class Tree {
private:
	Tree * P;
	Tree * L;
	Tree * R;

	int value;

public:
	Tree();
	Tree(int i);
	Tree(Tree * p);
	Tree(Tree * p, int i);
	~Tree();

	Tree * addLeft(int i = 0);
	Tree * addRight(int i = 0);
	Tree * left();
	Tree * right();

	int getValue();
	void setValue(int i);

};