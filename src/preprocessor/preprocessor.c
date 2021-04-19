#include "preprocessor.h"

#include "../um/um.h"

void init_interpreter(void) {
	um_init();
}

char* PreProcess(const char* INPUT) {
	char *TEMP = calloc(strlen(INPUT) + 1, sizeof(char)), *OUTPUT = NULL;

	TEMP = strdup(INPUT);

	while (strstr(TEMP, START_SEQUENCE)) {
		const size_t len_start = next_occur(TEMP, START_SEQUENCE),
			     len_end = next_occur(TEMP, END_SEQUENCE),
			     out_len = strlen(TEMP);

		char *start_string = calloc(len_start, sizeof(char)),
		     *code = calloc(len_end - len_start, sizeof(char)),
		     *end_string = calloc(out_len - len_end, sizeof(char)),
		     *result = NULL;

		/* copy the portion before code and escape sequnce */
		memcpy(start_string, TEMP, len_start);
		/* copy the code in between */
		memcpy(code,
		       TEMP + len_start + START_SEQUENCE_LEN,
		       len_end - len_start - START_SEQUENCE_LEN);
		/* copy the last remainder */
		end_string = strdup(TEMP + len_end + 2);

		/* call interpreter */
		/* TODO: handle error case */
		result = to_string(um_interpret_string(code).data, 0);

		/* realloc() with double capacity */
		if (len_start + strlen(result) + len_end > out_len) {
			TEMP = realloc(TEMP, out_len * 2);
		}

		sprintf(TEMP, "%s%s%s", start_string, result, end_string);
	}

	/* Free any excess memory */
	OUTPUT = calloc(strlen(TEMP) + 1, sizeof(char));
	OUTPUT = strdup(TEMP);
	free(TEMP);

	return OUTPUT;
}
