/// TestEntry: call_debug_so
/// Link: rk3399_face_detect_db_debug
/// Link: ${OpenCV_LIBS}
/// ----------------------

#include<stdio.h>

extern "C" {
	int debugPingPong(int ping);
}

int main() {
	int pingNumber = 63;
	printf("ping(%d) == %d\n", pingNumber, debugPingPong(pingNumber));
	printf("call debug shared library success!\n");
	return 0;
}
