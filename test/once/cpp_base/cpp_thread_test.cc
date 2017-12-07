/// TestEntry: cpp_thread
/// Link: ${CMAKE_THREAD_LIBS_INIT}
/// ----------------------

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<mutex>
#include<atomic>
#include "../../utilities/index.hpp"

#define SIZE 128

std::mutex mutexLocker;

int value = 0;

void* plusOne1000Times(void* ptr) {
	for(int i = 0; i < 10000; i ++ ) {
		mutexLocker.lock();
		value++;
		mutexLocker.unlock();
	}
}

int main() {
	pthread_t threads[10];
	for(int i = 0 ; i < 10 ; i ++ ){
		if(pthread_create(&threads[i], NULL, plusOne1000Times, NULL) != 0) {
			printf("Create thread %d failed!\n", i);
			return 1;
		}
	}

	for(int i = 0 ; i < 10 ; i ++ ){
		pthread_join(threads[i], NULL);
	}

	printf("value: %d\n", value);

	return testDone("C++ thread test");
}
