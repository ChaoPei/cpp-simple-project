//
// 确定系统的字节序
// 大端或者小端字节
//


#include <stdio.h>

// 因为是union, 所以value和byte共享内存
// 可以通过byte来访问value的高字节和低字节
typedef union {
	unsigned short int value;
	unsigned char bytes[2];
} test_type;

int main() {
	test_type typeorder;
	typeorder.value = 0xabcd;
	
	printf("test_type size: %d\n", sizeof(test_type));
	printf("byte[0]: %x\n", typeorder.bytes[0]);
	printf("byte[1]: %x\n", typeorder.bytes[1]);
	
	// 
	if(typeorder.bytes[0] == 0xcd && typeorder.bytes[1] == 0xab) {
		printf("this is a low endian machine\n");
	}

	return 0;
}


