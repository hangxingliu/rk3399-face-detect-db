#include<stdio.h>

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
