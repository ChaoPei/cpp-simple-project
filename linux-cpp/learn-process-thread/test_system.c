#include <stdlib.h>
#include <stdio.h>

int main(void) {
	int ret;

	printf("My pid: %d\n", getpid());
	ret = system("ping www.baidu.com -c 4");
	printf("ret: %d\n", getpid());

	return 0;
}

