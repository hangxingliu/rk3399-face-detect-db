/// TestEntry: call_debug_so
/// Link: rk3399_face_detect_db_debug
/// ----------------------

#include<stdio.h>

extern "C" {
	int face_ping_pong(int ping);
}

int main() {
	int pingNumber = 63;
	printf("ping(%d) == %d\n", pingNumber, face_ping_pong(pingNumber));
	printf("call debug shared library success!\n");
	return 0;
}
