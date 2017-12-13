#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#include "../log/log.hpp"

#ifndef UTILS_MEMORY_HPP
#define UTILS_MEMORY_HPP

typedef void (*MM_Revoke_Callback)(uint itemIndex);

template <class T>
class MemoryManager {
private:
	T* *list;
	uint listCapacity = 0;
	size_t allocated = 0;

	size_t maxAllocatedSize = 0;
	size_t eachItemSize = 0;

	MM_Revoke_Callback callback = nullptr;

	void revoke(uint itemIndex) {
		if(callback)
			(*callback)(itemIndex);
		free(list[itemIndex]);
		list[itemIndex] = nullptr;
		allocated -= eachItemSize;
	}
	int findRevokableItem() {
		for(uint i = 0 ; i < listCapacity ; i ++ )
			if(list[i])
				return (int) i;
		LOG_WARN_F("Can not find any revokable item for revoking memory");
		return -1;
	}

public:
	MemoryManager(uint _listCapacity,
		size_t _maxAllocatedSize) {

		list = (T**) malloc(sizeof(T*) * _listCapacity);
		listCapacity = _listCapacity;
		for(uint i = 0 ; i < _listCapacity ; i ++ )
			list[i] = nullptr;

		maxAllocatedSize = _maxAllocatedSize;
		eachItemSize = sizeof(T);
	}
	~MemoryManager() {
		for(uint i = 0 ; i < listCapacity ; i ++ )
			if(list[i])
				free(list[i]);
		free(list);
	}

	// Copy/= operation warning!
	MemoryManager(const MemoryManager& ori) {
		LOG_FATAL("Copying MemoryManager instance is invalid");
	}
	MemoryManager &operator =(const MemoryManager& ori) {
		LOG_FATAL("Copying MemoryManager instance is invalid");
		return *this;
	}

	void registerRevokeCallback(MM_Revoke_Callback _callback) {
		callback = _callback;
	}

	T* allocate(uint itemIndex) {
		if(list[itemIndex])
			return list[itemIndex];

		// should revoke
		if(allocated + eachItemSize > maxAllocatedSize) {
			int i = findRevokableItem();
			if(i >= 0)
				revoke((uint) i);
		}

		allocated += eachItemSize;
		return list[itemIndex] = (T*) malloc(eachItemSize);
	}

	T* at(uint itemIndex) {
		if(itemIndex < 0 || itemIndex >= listCapacity)
			return nullptr;
		return list[itemIndex];
	}
	T** getItemList() { return list; }
	uint getItemListCapacity() { return listCapacity; }
	long getAllocated() { return allocated; }
	long getMaxAllocatedSize() { return maxAllocatedSize; }
};

#endif
