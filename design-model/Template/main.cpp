#include "Template.hpp"

int main() {
	Resume *r1;
	r1 = new ResumeA();
	r1->fillResume();

	Resume *r2 = new ResumeB();
	r2->fillResume();

	delete r1;
	delete r2;
	r1 = r2 = NULL;
	return 0;
}