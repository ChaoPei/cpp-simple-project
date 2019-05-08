#include "Prototype.hpp"

int main() {
	Resume *r1 = new ResumeA("A");
	r1->show();
	Resume *r2 = new ResumeB("B");
	r2->show();
	Resume *r3 = r1->clone();
	r3->show();
	Resume *r4 = r2->clone();
	r4->show();

	// 删除r1, r2
	delete r1;
	delete r2;
	r1 = r2 = NULL;

	// r3,r4不受影响
	r3->show();
	r4->show();

	delete r3;
	delete r4;
	r3 = r4 = NULL;
}