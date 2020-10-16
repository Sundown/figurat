#include "postprocessor.h"

void write(char* filepath, char* contents) {
  FILE* output;
  if (!filepath || !strlen(filepath)) {
    puts("Write error");
    exit(1);
  }

  output = fopen(filepath, "w");

  if (!output) {
    puts("Open error");
    exit(1);
  }

  fprintf(output, "%s", contents);

  fclose(output);
}
