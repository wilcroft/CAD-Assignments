#include "tree.h"

Tree::Tree() {
	P=nullptr;
	L=nullptr;
	R=nullptr;

	value = 0;
}
Tree::Tree(int i) {
	P = nullptr;
	L = nullptr;
	R = nullptr;

	value = i;
}
Tree::Tree(Tree * p) {
	P = p;
	L = nullptr;
	R = nullptr;

	value = 0;
}
Tree::Tree(Tree * p, int i) {
	P = p;
	L = nullptr;
	R = nullptr;

	value = i;
}
Tree::~Tree() {
	if (L != nullptr) delete L;
	if (R != nullptr) delete R;

}
Tree * Tree::addLeft(int i) {
	if (L == nullptr) L = new Tree(this, i);
	else L->value = i;

	return L;
}
Tree * Tree::addRight(int i) {
	if (R == nullptr) R = new Tree(this, i);
	else R->value = i;

	return R;
}

Tree * Tree::left() { return L; }
Tree * Tree::right() { return R; }

int Tree::getValue() { return value; }
void Tree::setValue(int i) { value = i; }