#include "globals.h"

int main(int argc, char** argv) {
  size_t len;
  char* string;
  FILE* stream = NULL;

  /* load defaults into interpreter and init environment */
  INIT_INTERPRETER();

  if (argc < 2) {
    printf("Please include a path.\n");
  }

  stream = fopen(argv[1], "r");

  if (stream == NULL) {
    puts("Stream null");
  }

  fseek(stream, 0, SEEK_END);
  len = ftell(stream);
  fseek(stream, 0, SEEK_SET);

  string = (char*)malloc(len + 1);

  if (!string) {
    puts("File empty");
  }

  fread(string, 1, len, stream);
  string[len] = '\0';
  fclose(stream);

  if (string) {
    POSTPROCESS(PREPROCESS(string));
  }

  return 0;
}
