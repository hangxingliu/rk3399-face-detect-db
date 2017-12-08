#include<stdio.h>

/**
 * Integer number plus function
 * @param a integer A
 * @param b integer B
 * @return A+B
 */
int plus(int a,int b) {
	return a + b;
}

/**
 * This is an example doxygen comment
 * @return status code
 */
int main() {
	#ifdef NDEBUG
		printf("Release Mode\n");
	#else
		printf("Debug Mode\n");
	#endif

	#ifdef DEBUG
		printf("Debug Mode\n");
	#endif
	#ifdef RELEASE
		printf("Release Mode\n");
	#endif

	auto plus = [](int x, int y) {return x + y;};
	int arr[]{20, 30, 40};

	for(auto e: arr) {
		printf("item: %d\n", e);
	}

	printf("HelloWorld!(%d)\n", plus(20, 30));
	return 0;
}
