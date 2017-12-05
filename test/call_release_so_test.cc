#include<stdio.h>

extern "C" {
	int debugPingPong(int ping);
}

int main() {
	int pingNumber = 63;
	printf("ping(%d) == %d\n", pingNumber, debugPingPong(pingNumber));
	printf("call release shared library success!\n");
	return 0;
}
