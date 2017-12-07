#include<mutex>
#include "./_constant.hpp"

std::mutex __FRAME_SWITCHER_LOCK;

std::mutex __FRAME_ACCESS_LOCK[MAX_FRAME_DATA];

std::mutex __FACE_DETECT_EXTRACT;

std::mutex __DATABASE_LOCK;

void lockFrameSwitcher() { __FRAME_SWITCHER_LOCK.lock(); }
void unlockFrameSwitcher() { __FRAME_SWITCHER_LOCK.unlock(); }

void lockFrameAccess(int id) {
	if(id >= 0 && id < MAX_FRAME_DATA) {
		auto locker = &(__FRAME_ACCESS_LOCK[id]);
		locker->lock();
	}
}
void unlockFrameAccess(int id) {
	if(id >= 0 && id < MAX_FRAME_DATA) {
		auto locker = &(__FRAME_ACCESS_LOCK[id]);
		locker->unlock();
	}
}

void lockDetectAndExtract() { __FACE_DETECT_EXTRACT.lock(); }
void unlockDetectAndExtract() { __FACE_DETECT_EXTRACT.unlock(); }

void lockDatabase() { __DATABASE_LOCK.lock(); }
void unlockDatabase() { __DATABASE_LOCK.unlock(); }

