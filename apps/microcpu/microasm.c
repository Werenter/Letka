#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sq_io.h"
#include "logger.h"
#include "cpu_commands.h"

Status_type assemble_command(FILE *output_file, char *line, const char *name);
Status_type assemble_code(const char *path, char *input_data);
void fill_first_word(char *line);

void fill_first_word(char *line) {
	bool non_space_found = false;
	while(*line != '\0') {
		if(*line == ' ' || *line == '\n' || *line == '\r') {
			if(non_space_found) break;
		} else {
			non_space_found = true;
			*line = ' ';
		}
		line++;
	}
}

Status_type assemble_command(FILE *output_file, char *line, const char *name) {
	for(size_t i = 0; i < ARRAY_LENGTH(cpu_commands); i++) {
		if(strcmp(name, cpu_commands[i].name) == 0) {
			if(fwrite(&cpu_commands[i].opcode, sizeof(cpu_commands[i].opcode), 1, output_file) != 1) {
				LOG_ERROR("File write error\n");
				return STATUS_WRITE_ERROR;
			}

			for(size_t j = 0; j < cpu_commands[i].arg_count; j++) {
				fill_first_word(line);
				int arg;
				if(sscanf(line, "%i", &arg) != 1) return STATUS_SYNTAX_ERROR;

				if(fwrite(&arg, sizeof(arg), 1, output_file) != 1) {
					LOG_ERROR("File write error\n");
					return STATUS_WRITE_ERROR;
				}
			}
			return STATUS_OK;
		}
	}
	colored_fprintf(RED, stderr, "Command %s not exist\n", name);
	return STATUS_SYNTAX_ERROR;
}

Status_type assemble_code(const char *path, char *input_data) {
	FILE *output_file = fopen(path, "wb");
	if(output_file == NULL) {
		fputs("Can't open output file\n", stderr);
		return STATUS_FILE_OPEN_ERROR;
	}

	char *line_ptr = strtok(input_data, "\n\r");
	size_t line_number = 1;

	while(line_ptr != NULL) {
		char command[64] = {};

		char *comment_pos = strchr(line_ptr, ';');
		if(comment_pos != NULL) *comment_pos = '\0';

		if(sscanf(line_ptr, "%63s", command) != 1) {
			// Skip empty line
			line_number++;
			line_ptr = strtok(NULL, "\r\n");
			continue;
		}

		Status_type result = assemble_command(output_file, line_ptr, command);
		if(result != STATUS_OK) {
			colored_fprintf(RED, stderr, "Assembly error on %s:%lu\n", path, line_number);
			fprintf(stderr, "%s\n", line_ptr);
			fclose(output_file);
			return result;
		}

		line_number++;
		line_ptr = strtok(NULL, "\r\n");
	}
	fclose(output_file);
	return STATUS_OK;
}

int main(int argc, char **argv) {
	if(argc != 3) {
		puts("program input_file.microasm output_file.microcpu");
		return -1;
	}

	char *input_data = NULL;
	size_t filesize = 0;
	Status_type ret = read_file(argv[1], &input_data, &filesize);
	if(ret != STATUS_OK) {
		fputs("Can't read input file\n", stderr);
		return 1;
	}

	ret = assemble_code(argv[2], input_data);

	free(input_data);

	return (int)ret;
}
