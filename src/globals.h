#ifndef FIG_GLOBAL_HEADER
#define FIG_GLOBAL_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* ROOT_CONFIG;

char* PostProcess(const char* INPUT);
char* PreProcess(const char* INPUT);

void init_interpreter(void);

#define next_occur(haystack, needle) \
	((size_t)strstr(haystack, needle) - (size_t)haystack)

#endif /* FIG_GLOBAL_HEADER*/
