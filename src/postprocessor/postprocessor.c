#include "postprocessor.h"

char* POSTPROCESS(const char* INPUT) {
  char
    *TEMP = calloc(strlen(INPUT) + 1, sizeof(char)),
    *FILECONTENTS = NULL,
    *OUTPUT = NULL;

  TEMP = strdup(INPUT);

  while (strstr(TEMP, START_SEQUENCE)) {
    char* start_string = calloc(strcspn(TEMP, START_SEQUENCE), sizeof(char));
    char* path_string = calloc(
        strcspn(strstr(TEMP, START_SEQUENCE) + START_SEQUENCE_LEN, SEP_SEQUENCE),
        sizeof(char));
    char* contents_string = calloc(
        strcspn(strstr(TEMP, SEP_SEQUENCE) + SEP_SEQUENCE_LEN, END_SEQUENCE),
        sizeof(char));
    char* remainder_string = calloc(
        strlen(TEMP + strcspn(TEMP, END_SEQUENCE) + END_SEQUENCE_LEN),
        sizeof(char));

    size_t sep_len = strcspn(TEMP, SEP_SEQUENCE);

    memcpy(
      start_string,
      TEMP,
      strcspn(TEMP, START_SEQUENCE)
    );

    memcpy(
      path_string,
      TEMP + strcspn(TEMP, START_SEQUENCE) + START_SEQUENCE_LEN,
      strcspn(TEMP, SEP_SEQUENCE) - strcspn(TEMP, START_SEQUENCE) - START_SEQUENCE_LEN
    );

    memcpy(
      contents_string,
      TEMP + sep_len + SEP_SEQUENCE_LEN,
      strcspn(TEMP, END_SEQUENCE) - sep_len - SEP_SEQUENCE_LEN
    );

    memcpy(
      remainder_string,
      TEMP + strcspn(TEMP, END_SEQUENCE) + END_SEQUENCE_LEN,
      strlen(TEMP + strcspn(TEMP, END_SEQUENCE) + END_SEQUENCE_LEN)
    );

    FILECONTENTS = calloc(sizeof(char), strlen(contents_string) + 1);

    sprintf(FILECONTENTS, "%s", contents_string[0] == '\n' ? contents_string + 1 : contents_string);
    write(path_string, FILECONTENTS);

    sprintf(TEMP, "%s%s%s%s", start_string, path_string, contents_string, remainder_string);
  }

  /* Free any excess memory */
  OUTPUT = calloc(strlen(TEMP) + 1, sizeof(char));
  OUTPUT = strdup(TEMP);
  free(TEMP);

  return OUTPUT;
}
