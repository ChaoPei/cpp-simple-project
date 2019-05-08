#include "Builder.hpp"

int main() {
	MaleBuiler mb;
	Director director(&mb);
	director.create();
	return 0;
}