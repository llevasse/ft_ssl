#include "../include/ft_ssl.h"

char *read_file(char *path){
	char *input = 0x0;
	FILE *f = 0x0;
	if (path)
		f = fopen(path, "r");
	
	if (!f){
		fprintf(stderr, "Error opening input file : %s\n", strerror(errno));
		return (0x0);
	}
	if (fseek(f, 0, SEEK_END) == 0){
		long size = ftell(f);
		if (size < 0){
			fprintf(stderr, "Error opening input file : %s\n", strerror(errno));
			fclose(f);
			return (0x0);
		}
		input = malloc((size + 1) * sizeof(char));
		if (!input){
			fprintf(stderr, "Error opening input file : %s\n", strerror(errno));
			fclose(f);
			return (0x0);
		}
		if (fseek(f, 0L, SEEK_SET) != 0){
			fprintf(stderr, "Error opening input file : %s\n", strerror(errno));
			fclose(f);
			free(input);
			return (0x0);
		}
		size_t len = fread(input, sizeof(char), size, f);
		if (ferror(f) != 0) {
			fprintf(stderr, "Error reading input file : %s\n", strerror(errno));
			fclose(f);
			free(input);
			return (0x0);
		} 
		else {
			input[len] = '\0';
		}
	}
	fclose(f);
	return (input);
}