/// TestEntry: call_release_so
/// Link: rk3399_face_detect_db_release
/// Link: ${OpenCV_LIBS}
/// ----------------------

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
