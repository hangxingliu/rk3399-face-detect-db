/// TestEntry: call_release_so
/// Link: rk3399_face_detect_db_release
/// Link: ${OpenCV_LIBS}
/// ----------------------

#include<stdio.h>

extern "C" {
	int face_ping_pong(int ping);
}

int main() {
	int pingNumber = 63;
	printf("ping(%d) == %d\n", pingNumber, face_ping_pong(pingNumber));
	printf("call release shared library success!\n");
	return 0;
}
