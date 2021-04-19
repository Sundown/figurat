#include "postprocessor.h"

void write(char* path, char* data) {
	if (!path || !strlen(path)) { fputs("Write error", stderr), exit(1); }

	FILE* output = fopen(path, "w");

	if (!output) { fputs("Open error", stderr), exit(1); }

	fputs(data, output), fclose(output);
}
