#ifndef TEST_UTILS_MACRO_HPP
#define TEST_UTILS_MACRO_HPP

#define COLOR_GREEN "\x1b[32m"
#define COLOR_RED "\x1b[31m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

#define __T_PRINT(color, mark, name, ps, ...) {\
	char __t_print_str[128];\
	sprintf(__t_print_str, "%s[%c] %s: %s%s\n", color, mark, name, ps, COLOR_RESET);\
	printf(__t_print_str, __VA_ARGS__);\
	}

#define testFailedFormat(ps, ...) __T_PRINT(COLOR_RED, '-', "failed", ps, __VA_ARGS__);
#define testPassedFormat(ps, ...) __T_PRINT(COLOR_GREEN, '~', "passed", ps, __VA_ARGS__);

#endif // MACRO_HPP
