#ifndef FIG_POSTPROCESSOR_HEADER
#define FIG_POSTPROCESSOR_HEADER
#include "../globals.h"

#define START_SEQUENCE  "[["
#define   SEP_SEQUENCE  ":"
#define   END_SEQUENCE  "]]"

#define START_SEQUENCE_LEN  strlen(START_SEQUENCE)
#define   SEP_SEQUENCE_LEN  strlen(SEP_SEQUENCE)
#define   END_SEQUENCE_LEN  strlen(END_SEQUENCE)

void write(char* filepath, char* contents);

#endif /* FIG_POSTPROCESSOR_HEADER */
