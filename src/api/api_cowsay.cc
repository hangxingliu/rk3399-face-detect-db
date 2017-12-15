#include <stdio.h>
#include <stdlib.h>

#include "./api.hpp"

static const char* DETECT1_DIV_LINE =
	" -------------------------------------------------------------------";
static const char* DETECT1_STORY1 =
	"Hey! Do you know my real name is: face_detect_one_face_in_last_frame";
static const char* DETECT1_STORY2 =
	"Lol! This function name as short as you.";
static const char* DETECT1_STORY3 =
	"Don't invoke this API, It is deprecated.";
static const char* DETECT1_STORY[] = {
	DETECT1_STORY1, DETECT1_STORY2, DETECT1_STORY3};

int cowSayDeprecatedAPI() {
#ifndef RELEASE
	puts(DETECT1_DIV_LINE);
	printf("< %s >\n", DETECT1_STORY[rand() % 3]);
	puts(DETECT1_DIV_LINE);
	if(rand() % 16) puts("        \\   ^__^");
	if(rand() % 10) puts("         \\  (oo)\\_______");
	if(rand() % 14) puts("            (__)\\       )\\/\\");
	if(rand() % 20) puts("                ||----w |");
	if(rand() %  8) puts("                ||     ||");
	if(rand() %  4) puts("");
#endif
	return 0;
}
