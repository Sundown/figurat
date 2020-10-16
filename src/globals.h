#ifndef FIG_GLOBAL_HEADER
#define FIG_GLOBAL_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* ROOT_CONFIG;

char* PREPROCESS(const char* INPUT);
char* EXPAND(const char* INPUT);
char* POSTPROCESS(const char* INPUT);

void INIT_INTERPRETER();

#endif /* FIG_GLOBAL_HEADER*/
