#include "add/add.h"
#include "sub/sub.h"

#include <stdio.h>

int main() {
	int a = 1;
	int b = 2;

	float aa = 0.1;
	float bb = 0.2;

	printf("add_int : %d\n", add_int(a, b));
	printf("add_float : %f\n", add_float(aa, bb));
	printf("sub_int : %d\n", sub_int(a, b));
	printf("sub_float : %f\n", sub_float(aa, bb));

	return 0;
}
