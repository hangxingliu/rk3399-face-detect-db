#include<mutex>

#include "opencv2/opencv.hpp"

#ifndef FRAME_BUFFER_STRUCTURE_HPP
#define FRAME_BUFFER_STRUCTURE_HPP

#define FRAME_BUFFER_COUNT 32

class FrameBufferItem {
public:
	int id = -1;
	bool valid = false;
	std::mutex access;
	FrameBufferItem* last = nullptr;
	FrameBufferItem* next = nullptr;

	cv::Mat image;
	bool isBlurry;

	void lock() { access.lock(); }
	void unlock() { access.unlock(); }
	bool tryLock() { return access.try_lock(); }
};

class FrameBufferManager {
private:
	std::mutex invoke;
	FrameBufferItem* insertPoint;

	FrameBufferItem buffer[FRAME_BUFFER_COUNT];

public:
	FrameBufferManager() {
		const int border = FRAME_BUFFER_COUNT - 1;
		for(int i = 0 ; i <= border ; i ++ ) {
			auto item = &(buffer[i]);
			item->id = i;
			item->last = i == 0 ? &(buffer[border]) : &(buffer[i-1]);
			item->next = i == border ? &(buffer[0]) : &(buffer[i+1]);
		}
		insertPoint = &(buffer[i]);
		bufferWriter = &(buffer[i]);
	}

	FrameBufferItem* startInsert() {
		invoke.lock();

		if(!insertPoint->tryLock()) {
			invoke.unlock();

			return nullptr;// other is inserting
		}

		insertPoint->valid = false;

		invoke.unlock();
		return insertPoint;
	}
	void insertDone() {
		invoke.lock();

		insertPoint->valid = true;
		insertPoint->unlock();
		insertPoint = insertPoint->next;

		invoke.unlock();
	}
	int getLastFrameId() {
		invoke.lock();
		int id = insertPoint->last->id;
		invoke.unlock();

		return id;
	}

	FrameBufferItem* startAccess(int frameId) {
		if(frameId < 0 || frame >= FRAME_BUFFER_COUNT)
			return nullptr;

		auto it = &(buffer[frameId]);
		it->lock();
		return it;
	}
	void accessDone(FrameBufferItem* item) {
		item->unlock();
	}

}

#endif // FRAME_BUFFER_STRUCTURE_HPP
