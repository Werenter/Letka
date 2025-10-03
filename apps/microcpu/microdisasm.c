#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stack.h"
#include "logger.h"
#include "sq_io.h"
#include "cpu_commands.h"

static void disasm(int *instruction_data, size_t data_len, const char *path);
static int read_input(const char *path, int **input_data, size_t *input_size);

void disasm(int *instruction_data, size_t data_len, const char *path) {
	FILE *output_file = fopen(path, "wb");
	if(output_file == NULL) {
		fputs("Can't open output file\n", stderr);
		return;
	}

	for(size_t i = 0; i < data_len; i++) {
		bool command_found = false;

		for(size_t j = 0; j < ARRAY_LENGTH(cpu_commands); j++) {
			if(cpu_commands[j].opcode == instruction_data[i]) {
				command_found = true;
				fputs(cpu_commands[j].name, output_file);

				for(size_t k = 0; k < cpu_commands[j].arg_count && i < data_len; k++) {
					fprintf(output_file, " %i", instruction_data[++i]);
				}
				fputc('\n', output_file);
			}
		}

		if(!command_found) {
			fprintf(output_file, "; Illegal instruction: %i\n", instruction_data[i]);
			colored_fprintf(RED, stderr, "Illegal instruction: %i\n", instruction_data[i]);
		}
	}

	fclose(output_file);
}

static int read_input(const char *path, int **input_data, size_t *input_size) {
	FILE *input = fopen(path, "rb");
	if(input == NULL) {
		fputs("Input file open error\n", stderr);
		return 1;
	}

	long filesize = get_filesize(input);
	if(filesize < 0 || (size_t)filesize%sizeof(int) != 0) {
		fputs("Invalid bytecode file\n", stderr);
		fclose(input);
		return 2;
	}

	*input_data = malloc((size_t)filesize);
	if(*input_data == NULL) {
		fputs("Memory allocation error", stderr);
		fclose(input);
		return 3;
	}

	if(fread(*input_data, sizeof(int), (size_t)filesize/sizeof(int), input) != (size_t)filesize/sizeof(int)) {
		fputs("File read error", stderr);
		fclose(input);
		return 4;
	}

	*input_size = (size_t)filesize/sizeof(int);
	fclose(input);
	return 0;
}

int main(int argc, char **argv) {
	if(argc != 3) {
		puts("program input_file.microcpu disasm.microasm");
		return -1;
	}

	int *input_data = NULL;
	size_t input_size = 0;
	int ret = read_input(argv[1], &input_data, &input_size);
	if(ret != 0) return ret;

	disasm(input_data, input_size, argv[2]);

	free(input_data);
	return 0;
}
