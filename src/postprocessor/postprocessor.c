#include "postprocessor.h"

char* PostProcess(const char* input) {
	char *temp = strdup(input), *file_contents = NULL, *output = NULL;

	while (strstr(temp, START_SEQUENCE)) {
		char* start_string
		    = calloc(next_occur(temp, START_SEQUENCE), sizeof(char));
		char* path_string
		    = calloc(next_occur(strstr(temp, START_SEQUENCE)
					    + START_SEQUENCE_LEN,
					SEP_SEQUENCE),
			     sizeof(char));
		char* contents_string = calloc(
		    next_occur(strstr(temp, SEP_SEQUENCE) + SEP_SEQUENCE_LEN,
			       END_SEQUENCE),
		    sizeof(char));
		char* remainder_string
		    = calloc(strlen(temp + next_occur(temp, END_SEQUENCE)
				    + END_SEQUENCE_LEN),
			     sizeof(char));

		size_t sep_len = next_occur(temp, SEP_SEQUENCE);

		memcpy(start_string, temp, next_occur(temp, START_SEQUENCE));

		memcpy(path_string,
		       temp + next_occur(temp, START_SEQUENCE)
			   + START_SEQUENCE_LEN,
		       next_occur(temp, SEP_SEQUENCE)
			   - next_occur(temp, START_SEQUENCE)
			   - START_SEQUENCE_LEN);

		memcpy(contents_string,
		       temp + sep_len + SEP_SEQUENCE_LEN,
		       next_occur(temp, END_SEQUENCE) - sep_len
			   - SEP_SEQUENCE_LEN);

		memcpy(remainder_string,
		       temp + next_occur(temp, END_SEQUENCE) + END_SEQUENCE_LEN,
		       strlen(temp + next_occur(temp, END_SEQUENCE)
			      + END_SEQUENCE_LEN));

		file_contents
		    = calloc(sizeof(char), strlen(contents_string) + 1);

		sprintf(file_contents,
			"%s",
			contents_string[0] == '\n' ? contents_string + 1
						   : contents_string);
		write(path_string, file_contents);

		sprintf(temp,
			"%s%s%s%s",
			start_string,
			path_string,
			contents_string,
			remainder_string);
	}

	/* Free any excess memory */
	output = calloc(strlen(temp) + 1, sizeof(char));
	output = strdup(temp);
	free(temp);

	return output;
}
