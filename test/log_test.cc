#define DELAY_TEST 0

#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include "./utils/index.hpp"
#include "../src/log/log.hpp"

#define SIZE 128

int main() {
	char buf[SIZE];
	char buf2[SIZE];
	char* str = Log_getLogDatePrefix();
	strcpy(buf, str);

	if(strlen(buf) != 14) {
		char desc[SIZE];
		sprintf(desc, "strlen(logDatePrefix) != 14 . but it is %zu", strlen(buf));
		return testFailed(desc);
	}
	testPassed("strlen(logDatePrefix) == 14");
	printf("date: %s\n", buf);

#if DELAY_TEST
	//sleep micro seconds
	usleep(1100 * 1000); //sleep 1.1s

	str = Log_getLogDatePrefix();
	strcpy(buf2, str);
	printf("date: %s\n", buf2);

	int cmp = 13;
	if( !(buf[cmp] == '9' && buf2[cmp] == '0') &&
		!(buf[cmp] >= '0' && buf[cmp] < '9' && buf2[cmp] == buf[cmp] + 1))
		return testFailed("one second dealy test failed!");

	testPassed("one second dealy test");

#endif
	int arr[] = {100, 567, 888, -345, 9870, 2333, 520, 666666};
	int length = sizeof(arr) >> 2; // => size / 4
//	printf("siezof arr => %zu\n", sizeof(arr));
	Log_dumpIntArray(arr, length, 5, NULL);
	Log_dumpIntArray(arr, length, 4, "ArrayTest");
	Log_dumpIntArray(arr, length, 0, NULL);
	Log_dumpIntArray(arr, 0, 0, NULL);

	char arrChar[] = "HelloWorld!";
	length = sizeof(arrChar);
	Log_dumpByteArray(arrChar, length, 0, NULL);

	float arrFloat[] = {10.56f, 30.567f, 0.0f, 50.0f};
	length = sizeof(arrFloat) >> 2;
	Log_dumpFloatArray(arrFloat, length, 0, NULL);


	Log_dumpInt(100, NULL);
	Log_dumpFloat(1001.0f, "Float1001");

	Log_dumpString("HelloWorld", NULL);
	Log_dumpString("", "LongLongLongName");
	Log_dumpString("LongLongLongName", "1234567890-1234567890-1234567890-1234567890-1234567890-1234567890");

	Log_warn("FBI!");
	Log_info("torrent");

	LOG_WARN("FBI! --macro");
	LOG_INFO("torrent --macro");

	return testDone("Log module test");
}
