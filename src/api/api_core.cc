#include "./api.hpp"

bool APITracker_hideGetFrameLog = false;
bool APITracker_hideApiInvoke = false;

int face_set_debug_log(
	int apiGetFrameLog,
	int apiInvokeLog) {

	APITracker_hideGetFrameLog = !apiGetFrameLog;
	APITracker_hideApiInvoke = !apiInvokeLog;

	return API_OK;
}

/**
 * Debug API to
 * @param ping
 * @return ping+1
 */
int face_ping_pong(int ping){
	LOG_API_INVOKE("ping_pong", "%d", ping);
	return ping + 1;
}

